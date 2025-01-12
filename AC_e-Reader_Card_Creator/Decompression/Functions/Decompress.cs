using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using AC_e_Reader_Card_Creator.References;

namespace AC_e_Reader_Card_Creator.Decompression.Functions
{
    internal class Decompress
    {
        public static void BINtoVPK(string filePath, byte[] delimiter, string outputDirectory)
        {
            List<byte> buffer = [];
            int chunkNumber = 1;

            using FileStream fs = new(filePath, FileMode.Open, FileAccess.Read);
            using BinaryReader reader = new(fs);
            while (reader.BaseStream.Position != reader.BaseStream.Length)
            {
                byte b = reader.ReadByte();
                buffer.Add(b);

                if (buffer.Count >= delimiter.Length &&
                    buffer.Skip(buffer.Count - delimiter.Length).SequenceEqual(delimiter))
                {
                    WriteToFile(buffer.Take(buffer.Count - delimiter.Length).ToArray(), ref chunkNumber, outputDirectory, delimiter);
                    buffer.Clear();
                }
            }

            if (buffer.Count > 0)
            {
                WriteToFile([.. buffer], ref chunkNumber, outputDirectory, delimiter);
            }
        }

        public static void RAWtoBIN(string filePath)
        {
            try
            {
                int result = NedcLib.raw2bin(filePath, Common.COMPRESSED_BIN);
                switch (result)
                {
                    case 0:
                        break;
                    case -1:
                        throw new Exception("Unable to open input file");
                    case -2:
                        throw new Exception("Invalid raw file");
                    case -3:
                        throw new Exception("Unable to decode raw file to bin");
                    case -4:
                        throw new Exception("Unable to write to output file");
                    default:
                        throw new Exception($"raw2bin failed with error code {result}");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error processing file {filePath}: {ex.Message}");
            }
        }

        public static void VPK_Decompress()
        {
            string folderPath = Common.VPK_OUTPUT;
            string[] vpk_file_paths = Directory.GetFiles(folderPath);

            int iteration = 1;

            foreach (string vpk_file in vpk_file_paths)
            {
                try
                {
                    if (iteration == 1)
                    {
                        iteration++;
                        continue;
                    }
                    else if (iteration == 2)
                    {
                        NedcLib.VpkDecompress(vpk_file, Common.DECOMPRESSED_GBA);
                        iteration++;
                    }
                    else if (iteration == 3)
                    {
                        NedcLib.VpkDecompress(vpk_file, Common.DECOMPRESSED_GCN);
                        iteration++;
                    }
                    else
                    {
                        throw new Exception("Too many VPK files found");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error processing file {vpk_file}: {ex.Message}");
                }
            }
        }

        private static void WriteToFile(byte[] data, ref int chunkNumber, string outputDirectory, byte[] delimiter)
        {
            string fileName = "";

            switch (chunkNumber)
            {
                case 1:
                    fileName = Path.Combine(outputDirectory, $"1_VPK_Header.vpk");
                    break;
                case 2:
                    fileName = Path.Combine(outputDirectory, $"2_VPK_GBA.vpk");
                    break;
                case 3:
                    fileName = Path.Combine(outputDirectory, $"3_VPK_GCN.vpk");
                    break;
                default:
                    break;
            }

            using (FileStream outFile = new(fileName, FileMode.Create, FileAccess.Write))
            {
                if (chunkNumber != 1)
                {
                    outFile.Write(delimiter, 0, delimiter.Length);
                    outFile.Write(data, 0, data.Length);
                }
                else
                {
                    outFile.Write(data, 0, data.Length);
                }
            }
            chunkNumber++;
        }
    }
}
