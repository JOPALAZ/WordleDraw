namespace WordleDrawBackendWebApi.Models
{
    public class NoSolutionException(string msg) : Exception(msg)
    {
    }
}
