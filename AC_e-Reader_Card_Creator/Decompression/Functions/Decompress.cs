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
            List<byte> buffer = new List<byte>();
            int chunkNumber = 1;

            using (FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read))
            using (BinaryReader reader = new BinaryReader(fs))
            {
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
                    WriteToFile(buffer.ToArray(), ref chunkNumber, outputDirectory, delimiter);
                }
            }
        }

        public static void RAWtoBIN(string filePath)
        {
            try
            {
                ProcessStartInfo raw_to_bin = new ProcessStartInfo
                {
                    FileName = Common.NEDCENC,
                    Arguments = Common.NEDCENCE_ARGS_DECOMP(filePath),
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                };

                using (Process process = Process.Start(raw_to_bin)) { process.WaitForExit(); }
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
                    else
                    {
                        ProcessStartInfo vpk_to_dec = new ProcessStartInfo
                        {
                            FileName = Common.NEVPK,
                            Arguments = Common.NEVPK_ARGS_DECOMP(vpk_file, iteration),
                            UseShellExecute = false,
                            RedirectStandardOutput = true,
                            CreateNoWindow = true
                        };

                        using (Process process = Process.Start(vpk_to_dec)) { process.WaitForExit(); }

                        iteration++;
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
            
            using (FileStream outFile = new FileStream(fileName, FileMode.Create, FileAccess.Write))
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
