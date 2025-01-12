using System;
using System.Runtime.InteropServices;
using System.IO;

namespace AC_e_Reader_Card_Creator.Decompression.Functions
{
    internal partial class NedcLib
    {
        [LibraryImport("nedclib", StringMarshalling = StringMarshalling.Utf8, SetLastError = true)]
        internal static partial int is_bmp(string path);
        [LibraryImport("nedclib", StringMarshalling = StringMarshalling.Utf8, SetLastError = true)]
        internal static partial int bin2raw(string binfile, string rawfile);
        [LibraryImport("nedclib", StringMarshalling = StringMarshalling.Utf8, SetLastError = true)]
        internal static partial int raw2bin(string rawfile, string binfile);
        [LibraryImport("nedclib", StringMarshalling = StringMarshalling.Utf8, SetLastError = true)]
        private static partial int vpk_decompress(IntPtr buf, IntPtr fileHandle);
        [LibraryImport("nedclib", StringMarshalling = StringMarshalling.Utf8, SetLastError = true)]
        private static partial int NVPK_compress(IntPtr buf, int size, int compression_level, int lzwindow, int lzsize, int method, IntPtr fileHandle, IntPtr bitdata);
        [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Ansi, SetLastError = true)]
        private delegate int raw2bmp(string rawfile, string bmpfile);

        internal class UnmanagedFile : SafeHandle
        {
            [DllImport("nedclib", SetLastError = true, CharSet = CharSet.Ansi)]
            private static extern IntPtr nedc_fopen(string filename, string mode);
            [DllImport("nedclib", SetLastError = true, CharSet = CharSet.Ansi)]
            private static extern int nedc_fclose(IntPtr Handle);
            public UnmanagedFile(string path, string mode) : base(IntPtr.Zero, true)
            {
                if (string.IsNullOrEmpty(path)) throw new ArgumentException("File path cannot be null or empty.");
                if ((handle = nedc_fopen(path, mode)) == IntPtr.Zero)
                {
                    throw new IOException($"Failed to open file '{path}'.");
                }
            }

            public override bool IsInvalid { get => handle == IntPtr.Zero; }

            protected override bool ReleaseHandle()
            {
                if (handle == IntPtr.Zero)
                {
                    return true;
                }
                return nedc_fclose(handle) == 0;
            }
        }

        internal enum CompressionMethod
        {
            Store = 0,
            Fast = 1,
            Max = 2
        }

        internal static void Raw2Bmp(string rawfile, string bmpfile, int DPI)
        {
            IntPtr nedclib = NativeLibrary.Load("nedclib");
            IntPtr raw2bmpHandle = NativeLibrary.GetExport(nedclib, "raw2bmp");
            IntPtr dpiHandle = NativeLibrary.GetExport(nedclib, "dpi_multiplier");
            int dpiMultiplier = (int)Math.Pow(2.0, Math.Floor(Math.Log2(DPI / 300)));
            Marshal.WriteInt32(dpiHandle, dpiMultiplier);
            raw2bmp raw2bmpDelegate = Marshal.GetDelegateForFunctionPointer<raw2bmp>(raw2bmpHandle);
            int result = raw2bmpDelegate(rawfile, bmpfile);
            NativeLibrary.Free(nedclib);
            if (result != 0)
            {
                throw new Exception($"raw2bmp failed with error code {result}");
            }
        }

        internal static void VpkCompress(string inputFilePath, string outputFilePath, int compressionLevel = 2, int lzwindow = 4096, int lzsize = 256, CompressionMethod method = CompressionMethod.Max)
        {
            byte[] inputBytes = File.ReadAllBytes(inputFilePath);
            File.Create(outputFilePath).Close();
            using (UnmanagedFile fileHandle = new(outputFilePath, "wb"))
            {
                var inputBytesHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
                int result = NVPK_compress(inputBytesHandle.AddrOfPinnedObject(), inputBytes.Length, compressionLevel, lzwindow, lzsize, (int)method, fileHandle.DangerousGetHandle(), IntPtr.Zero);
                inputBytesHandle.Free();
                if (result != 0)
                {
                    throw new Exception($"VPK_compress failed with error code {result}");
                }
            };
        }

        internal static void VpkDecompress(string inputFilePath, string outputFilePath)
        {
            byte[] inputBytes = File.ReadAllBytes(inputFilePath);
            File.Create(outputFilePath).Close();
            using UnmanagedFile fileHandle = new UnmanagedFile(outputFilePath, "wb");
            var inputBytesHandle = GCHandle.Alloc(inputBytes, GCHandleType.Pinned);
            int result = vpk_decompress(inputBytesHandle.AddrOfPinnedObject(), fileHandle.DangerousGetHandle());
            inputBytesHandle.Free();
            if (result != 0)
            {
                throw new Exception($"vpk_decompress failed with error code {result}");
            }
        }
    }
}