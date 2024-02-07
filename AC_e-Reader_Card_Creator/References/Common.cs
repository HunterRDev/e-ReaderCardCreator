using System;
using System.IO;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace AC_e_Reader_Card_Creator.References
{
    internal class Common
    {
        /* 
        a bunch of public reference strings for directory pathing -- not ideal
        the program did not like working with C/CPP source code or its converted C# equivalent
        ideally fix this and store with just variables, but it could be fine to give users easy access to the decomp'd files 
        */

        public static string VERSION = "v1.0.0";

        public static string CREDIT = "AC e-Reader Character Card Creator\n" +
                                      VERSION + "\n" +
                                      "Created by: Hunter R.\n\n" +
                                      "External tools by:\n" +
                                      "CaitSith2\n" +
                                      "Tim Schuerewegen\n" +
                                      "BlackShark";

        public static string STATIONERY_LIST = @"Project Files\References\AC_Stationery_List.txt";
        public static string ITEM_LIST = @"Project Files\References\AC_Item_List.txt";
        public static string SENDER_LIST = @"Project Files\References\AC_Sender_List.txt";
        public static string GREETING_LIST = @"Project Files\References\AC_Greeting_List.txt";
        public static string SP_CHAR_LIST = @"Project Files\References\AC_Special_Char.txt";

        public static string NEVPK = @"Project Files\Decompression\External\nevpk.exe";
        public static string NEDCENC = @"Project Files\Decompression\External\nedcenc.exe";

        public static string VPK_GBA_BLANK = @"Project Files\References\GBA_Blank.vpk";
        public static string VPK_HEADER_BLANK = @"Project Files\References\Header_Blank.VPK";
        public static string HEADERFIX = @"Project Files\Decompression\External\headerfix.exe";
        public static string RAW2BMP = @"Project Files\Decompression\External\raw2bmp.exe";

        public static string RAW_ECARD = @"Project Files\Decompression\eCard\raw\eCard.raw";
        public static string COMPRESSED_BIN = @"Project Files\Decompression\eCard\bin\card_data.bin";
        public static string VPK_HEADER = @"Project Files\Decompression\eCard\vpk\1_VPK_Header.vpk";
        public static string VPK_GBA = @"Project Files\Decompression\eCard\vpk\2_VPK_GBA.vpk";
        public static string VPK_GCN = @"Project Files\Decompression\eCard\vpk\3_VPK_GCN.vpk";
        public static string DECOMPRESSED_GBA = @"Project Files\Decompression\eCard\dec\decompressed_data_GBA.bin";
        public static string DECOMPRESSED_GCN = @"Project Files\Decompression\eCard\dec\decompressed_data_GCN.bin";
        public static string DECOMPRESSED_DIR = @"Project Files\Decompression\eCard\";
        public static string BMP_DOTCODE = @"Project Files\Decompression\eCard\bmp\dotcode";
        public static string GCN_LETTER_DATA = @"Project Files\Decompression\eCard\dec\decompressed_data_GCN.bin";

        public static byte[] VPK_DELIMITER = [0x76, 0x70, 0x6B, 0x30]; // "vpk0"
        public static string RAW_OUTPUT = @"Project Files\Decompression\eCard\raw\";
        public static string BMP_OUTPUT = @"Project Files\Decompression\eCard\bmp\";
        public static string VPK_OUTPUT = @"Project Files\Decompression\eCard\vpk\";
        public static string BIN_OUTPUT = @"Project Files\Decompression\eCard\bin\";
        public static string DEC_OUTPUT = @"Project Files\Decompression\eCard\dec\";

        // TODO: edit for GBA support
        public static string NEVPK_ARGS_COMP(string filePath)
        {
            return $"-i \"{filePath}\" -o \"{VPK_GCN}\" -v -c";
        }
        public static string NEVPK_ARGS_DECOMP(string filePath, int iteration)
        {
            switch (iteration)
            {
                case 1:
                    break;
                case 2:
                    // TODO: implement GBA data support
                    return $"-i \"{filePath}\" -o \"{DECOMPRESSED_GBA}\" -v -d";
                case 3:
                    return $"-i \"{filePath}\" -o \"{DECOMPRESSED_GCN}\" -v -d";
            }

            return null;
        }
        public static string NEDCENC_ARGS_COMP(string filePath)
        {
            return $"-e -i \"{COMPRESSED_BIN}\" -o \"{filePath}";
        }
        public static string NEDCENCE_ARGS_DECOMP(string filePath)
        {
            return $" -d -i \"{filePath}\" -o \"{COMPRESSED_BIN}";
        }
        public static string RAW2BMP_ARGS(string filePath, int DPI)
        {
            return $" -i \"{RAW_ECARD}\" -o \"{filePath}\" -dpi {DPI}";
        }

        public static string LookupListValue(string stationeryID, string list)
        {
            try
            {
                string[] lines = File.ReadAllLines(list);

                foreach (string line in lines)
                {
                    string[] parts = line.Split(',');

                    if (parts[0].Trim() == stationeryID)
                    {
                        return parts[1].Trim();
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }

            return null;
        }

        public static void HandleLetterBody(List<Label> letter_lines, TextBox letter_body, int[] fontColor)
        {

            if (string.IsNullOrEmpty(letter_body.Text))
            {
                for (int i = 1; i <= 6; i++)
                {
                    letter_lines[i].Text = null;
                }
                return;
            }

            foreach (Label letter_line in letter_lines)
            {
                letter_line.ForeColor = Color.FromArgb(fontColor[0], fontColor[1], fontColor[2]);
            }

            string[] delimiter = ["\n"];
            string[] split_letter_body = letter_body.Text.Split(delimiter, StringSplitOptions.None);

            int line_index = 1;
            foreach (string line in split_letter_body)
            {
                string handled_text = line.Replace("&", "&&");
                letter_lines[line_index].Text = handled_text;
                line_index++;
            }
        }

        public static void LoadBlankVPKs()
        {
            try
            {
                if (!Directory.Exists(VPK_OUTPUT))
                {
                    MessageBox.Show($"The specified directory does not exist: {VPK_OUTPUT}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                string headerPath = VPK_HEADER;
                string GBAPath = VPK_GBA;

                File.Copy(VPK_HEADER_BLANK, headerPath, true);
                File.Copy(VPK_GBA_BLANK, GBAPath, true);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
