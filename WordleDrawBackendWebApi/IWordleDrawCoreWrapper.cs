using WordleDrawBackendWebApi.Models;

namespace WordleDrawBackendWebApi
{
    public interface IWordleDrawCoreWrapper : IDisposable
    {
        public ICollection<string> GetWordsSolution(DrawOptions drawOptions);
    }
}
