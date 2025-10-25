using System.ComponentModel;
using System.Reflection.Metadata;
using System.Runtime.InteropServices;
using System.Text;
using WordleDrawBackendWebApi.Models;

namespace WordleDrawBackendWebApi
{
    public class WordleDrawCoreWrapper : IWordleDrawCoreWrapper
    {
        private const string DllName = "WordleDrawCore.dll";
        private bool disposed = false;
        private IntPtr handle = IntPtr.Zero;

        public WordleDrawCoreWrapper(string path) 
        {
            handle = CreateWordleDrawCore(path);
        }
        public ICollection<string> GetWordsSolution(DrawOptions drawOptions)
        {
            bool[] bitmap = drawOptions.grid.SelectMany(a => a.Select(b => b == 1)).ToArray();
            int validatorType = (int)Enum.Parse<ValidatorType>(drawOptions.enumValue);

            var strBuilder = new StringBuilder(1024);
            int responseCode;
            string outputString;

            try
            {
                responseCode = GetWordsForBitmap(
                    handle,
                    bitmap,
                    validatorType,
                    drawOptions.answer.ToLowerInvariant(),
                    strBuilder,
                    strBuilder.Capacity
                );
                outputString = strBuilder.ToString();
            }
            catch (DllNotFoundException ex)
            {
                throw new Exception("CRITICAL: The native library 'WordleDrawCore.dll' was not found.", ex);
            }
            catch (SEHException ex)
            {
                throw new Exception("CRITICAL: The native C++ library crashed unexpectedly (SEHException).", ex);
            }

            if (responseCode < 0)
            {
                switch (responseCode)
                {
                    case -2:
                        throw new InvalidOperationException($"C++ Error: The output buffer was too small. (Message: {outputString})");
                    case -3:
                        throw new NoSolutionException(outputString);
                    case -10:
                        throw new ArgumentException($"C++ Error: Bad arguments were passed to the native function. (Message: {outputString})");
                    default:
                        throw new Exception($"An unknown error occurred in the native library. Code: {responseCode}, Message: {outputString}");
                }
            }

            return outputString.Trim().ToUpperInvariant().Split(',').ToList();
        }
        public void Dispose()
        {
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
        protected virtual void Dispose(bool disposing)
        {
            if (!this.disposed)
            {
                DeleteWordleDrawCore(handle);
                handle = IntPtr.Zero;

                disposed = true;
            }
        }

        ~WordleDrawCoreWrapper()
        {
            Dispose(disposing: false);
        }

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateWordleDrawCore([MarshalAs(UnmanagedType.LPStr)] string pathToReferences);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeleteWordleDrawCore(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetWordsForBitmap(
            IntPtr instance,
            [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I1)]
            bool[] bitmap,
            int validatorType,
            [MarshalAs(UnmanagedType.LPStr)]
            string answer,
            [MarshalAs(UnmanagedType.LPStr)]
            StringBuilder outputBuffer,
            int bufferSize);
    }
}
