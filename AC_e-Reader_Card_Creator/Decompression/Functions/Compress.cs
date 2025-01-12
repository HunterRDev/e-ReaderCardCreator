using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;
using System.Windows.Forms;
using AC_e_Reader_Card_Creator.References;

namespace AC_e_Reader_Card_Creator.Decompression.Functions
{
    internal class Compress
    {
        public static void DECtoVPK(string greeting, string body, string closing, string stationery, string sender, string gift)
        {
            string new_greeting = greeting;
            int player_index = greeting.IndexOf("<Player>");

            if (player_index != -1)
            {
                new_greeting = string.Concat(greeting.AsSpan(0, player_index), greeting.AsSpan(player_index + 8));
            }

            new_greeting = ACAsciiToBytes(new_greeting.PadRight(24, ' '));

            string new_body = body.Replace('\r'.ToString(), "").PadRight(192, ' ');
            new_body = ACAsciiToBytes(new_body);

            // should only be true if letter uses special characters
            if (new_body.Length < 384)
            {
                while (new_body.Length < 384)
                {
                    new_body += "20";
                }
            }
            else
            {
                // failsafe -- if programmed correctly, this should never run
                new_body = new_body[..384];
            }

            string new_closing = closing.PadRight(31, ' ');
            new_closing = ACAsciiToBytes(new_closing);

            Dictionary<string, string> StationeryMappings = ReadMappings(Common.STATIONERY_LIST);
            string new_stationery = StationeryMappings[stationery][2..];

            Dictionary<string, string> SenderMappings = ReadMappings(Common.SENDER_LIST);
            string new_sender = SenderMappings[sender][2..];

            string final = player_index.ToString("X2") + Random2ByteHex();

            string bytesToWrite = new_greeting + new_body + new_closing + new_stationery + gift + new_sender + final;

            byte[] body_bytes = HexStringToByteArray(bytesToWrite);
            string filePath = Common.DECOMPRESSED_GCN;
            File.WriteAllBytes(filePath, body_bytes);

            NedcLib.VpkCompress(Common.DECOMPRESSED_GCN, Common.VPK_GCN);

            FixVPKChecksums();
        }

        private static string Random2ByteHex()
        {
            Random random = new();
            int randomNumber = random.Next(0, 130);
            return randomNumber.ToString("X4");
        }

        public static void VPKtoBIN()
        {
            if (File.Exists(Common.COMPRESSED_BIN))
            {
                File.Delete(Common.COMPRESSED_BIN);
            }

            string[] VPKfiles = [Common.VPK_HEADER, Common.VPK_GBA, Common.VPK_GCN];

            using (FileStream NewBIN = new(Common.COMPRESSED_BIN, FileMode.CreateNew))
            {
                foreach (var sourceFile in VPKfiles)
                {
                    byte[] fileContent = File.ReadAllBytes(sourceFile);
                    NewBIN.Write(fileContent, 0, fileContent.Length);
                }

                long currentBINSize = NewBIN.Length;
                long paddingSize = 2112 - currentBINSize;

                if (paddingSize > 0)
                {
                    NewBIN.Seek(0, SeekOrigin.End);

                    // creating a padding buffer of 0x00 bytes to fill 2112 bytes
                    byte[] padding = new byte[paddingSize];
                    NewBIN.Write(padding, 0, padding.Length);
                }

                HeaderFix.FixHeaderChecksums(NewBIN);
            }
        }

        private static void BINtoRAW(string inputFilePath, string outputFilePath)
        {
            int result = NedcLib.bin2raw(inputFilePath, outputFilePath);
            switch (result)
            {
                case 0:
                    break;
                case -1:
                    throw new Exception("Unable to open input file");
                case -2:
                    throw new Exception("Invalid bin file");
                case -3:
                    throw new Exception("Unable to encode bin file to raw");
                case -4:
                    throw new Exception("Unable to write to output file");
                default:
                    throw new Exception($"bin2raw failed with error code {result}");
            }
        }

        public static void BINtoRAW(bool custom)
        {
            if (custom)
            {
                SaveFileDialog saveRAWFile = new()
                {
                    Filter = "RAW files (*.raw)|*.raw",
                    Title = "Save .RAW",
                    FileName = "My-Custom-eCard.raw"
                };
                if (saveRAWFile.ShowDialog() == DialogResult.OK)
                {
                    BINtoRAW(Common.COMPRESSED_BIN, saveRAWFile.FileName);
                }
            }
            else
            {
                BINtoRAW(Common.COMPRESSED_BIN, Common.RAW_ECARD);
            }
        }

        static Dictionary<string, string> ReadMappings(string filePath)
        {
            var map = new Dictionary<string, string>();
            foreach (var line in File.ReadAllLines(filePath))
            {
                var parts = line.Split(',');
                if (parts.Length >= 2)
                {
                    string hexKey = parts[0].Trim();
                    string value = parts[1].Trim().Trim('\'');
                    map[value] = hexKey;
                }
            }
            return map;
        }

        private static void FixVPKChecksums()
        {
            try
            {
                FileInfo fileInfo = new(Common.VPK_GCN);
                long GCN_VPK_Length = 0;

                if (fileInfo.Exists)
                {
                    GCN_VPK_Length = fileInfo.Length;
                }
                else
                {
                    MessageBox.Show("ERROR: Failed to fetch GCN VPK");
                    return;
                }

                string GCN_VPK_Hex_Length = GCN_VPK_Length.ToString("X").PadLeft(4, '0');

                if (!File.Exists(Common.VPK_GBA))
                {
                    MessageBox.Show("ERROR: Failed to fetch GBA VPK");
                    return;
                }
                byte[] fileContents = File.ReadAllBytes(Common.VPK_GBA);

                if (fileContents.Length < 2)
                {
                    MessageBox.Show("ERROR: GBA VPK exists, but is smaller in size than expected");
                    return;
                }

                byte[] GCN_FileSize_Checksum = HexStringToByteArray(GCN_VPK_Hex_Length);

                fileContents[^2] = GCN_FileSize_Checksum[1];
                fileContents[^1] = GCN_FileSize_Checksum[0];

                File.WriteAllBytes(Common.VPK_GBA, fileContents);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An error occurred: {ex.Message}");
            }
        }

        public static string ACAsciiToBytes(string input)
        {
            Dictionary<string, string> map = ReadMappings(Common.SP_CHAR_LIST);
            var sb = new StringBuilder();
            var textElementEnumerator = StringInfo.GetTextElementEnumerator(input);

            // a bit hacky and inefficient, but should work 
            while (textElementEnumerator.MoveNext())
            {
                string textElement = textElementEnumerator.GetTextElement();
                if (textElement == "\n")
                {
                    sb.Append("CD");
                }
                else if (textElement.Length == 1)
                {
                    if (map.TryGetValue(textElement, out string value))
                    {
                        sb.Append(value[2..]);
                    }
                    else
                    {
                        char c = textElement[0];
                        if ((byte)c >= 32 && (byte)c <= 127)
                        {
                            byte char_byte = (byte)c;
                            sb.Append(char_byte.ToString("X2"));
                        }
                        else
                        {
                            MessageBox.Show("An unexpected error occurred!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                }
                else if (map.TryGetValue(textElement, out string value))
                {
                    sb.Append(value.AsSpan(2));
                }
                else
                {
                    MessageBox.Show("Test!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }

            return sb.ToString().Trim();
        }

        static byte[] HexStringToByteArray(string hex)
        {
            int numberChars = hex.Length;
            byte[] bytes = new byte[numberChars / 2];
            for (int i = 0; i < numberChars; i += 2)
            {
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            }
            return bytes;
        }




    }
}
