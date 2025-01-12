using System;
using System.IO;

namespace AC_e_Reader_Card_Creator.Decompression.Functions
{
    internal class HeaderFix
    {
        private static ushort GetDataChecksum(byte[] data)
        {
            ushort sum = 0;
            for (int i = 0; i < data.Length; i += 2)
            {
                sum += BitConverter.ToUInt16(data, i);
            }
            return (ushort)(~sum & 0xFFFF);
        }

        private static byte GetHeaderChecksum(byte[] header)
        {
            ushort xor = 0;
            xor ^= header[0xC];
            xor ^= header[0xD];
            xor ^= header[0x10];
            xor ^= header[0x11];
            for (int i = 0x26; i < 0x2D; i++)
            {
                xor ^= header[i];
            }
            return (byte)(xor & 0xFF);
        }

        private static byte GetGlobalChecksum(byte[] header, byte[] data)
        {
            short sum = 0;

            for (int i = 0; i < 0x2F; i++)
            {
                sum += header[i];
            }

            for (int i = 0; i < data.Length / 0x30; i++)
            {
                short xor = 0;
                for (int j = 0; j < 0x30; j++)
                {
                    xor ^= data[(i * 0x30) + j];
                }
                sum += xor;
            }
            return (byte)(~sum & 0xFF);
        }

        public static void FixHeaderChecksums(FileStream fileStream)
        {
            long cardSize = fileStream.Seek(0, SeekOrigin.End);
            const int headerSize = 0x30;
            fileStream.Seek(0, SeekOrigin.Begin);
            if (cardSize % 0x30 != 0 || cardSize <= 0x30)
            {
                throw new Exception("Invalid card size");
            }
            byte[] header = new byte[headerSize];
            fileStream.Read(header, 0, headerSize);
            fileStream.Seek(0, SeekOrigin.Begin);
            byte[] signature = [0x4E, 0x49, 0x4E, 0x54, 0x45, 0x4E, 0x44, 0x4F]; // NINTENDO
            for (int i = 0; i < 8; i++)
            {
                if (header[0x1A + i] != signature[i])
                {
                    throw new Exception("Invalid e-Card signature");
                }
            }
            int dataSize = (header[0x06] << 8) + header[0x07];
            byte[] data = new byte[dataSize];
            fileStream.Seek(headerSize, SeekOrigin.Begin);
            fileStream.Read(data, 0, dataSize);

            ushort dataChecksum = GetDataChecksum(data);
            header[0x13] = (byte)(dataChecksum >> 8);
            header[0x14] = (byte)(dataChecksum & 0xFF);

            byte headerChecksum = GetHeaderChecksum(header);
            header[0x2E] = headerChecksum;

            byte globalChecksum = GetGlobalChecksum(header, data);
            header[0x2F] = globalChecksum;
            fileStream.Seek(0, SeekOrigin.Begin);
            fileStream.Write(header, 0, headerSize);
        }
    }
}