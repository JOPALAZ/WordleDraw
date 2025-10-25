using Microsoft.AspNetCore.Mvc;
using WordleDrawBackendWebApi.Models; // Assuming you have this
using System;
using System.Net.Http;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace WordleDrawBackendWebApi.Controllers
{

    //TODO USE LOGGER
    [ApiController]
    [Route("/getAnswerWord")]
    public class AnswerController : ControllerBase
    {
        [HttpGet]
        public async Task<IActionResult> getAnswerWord()
        {
            try
            {
                string answer = await WordleClient.GetCurrentAnswerWordAsync();

                if (answer == null)
                {
                    return StatusCode(500, "Failed to fetch Wordle answer from NYT.");
                }

                return Ok(answer);
            }
            catch (Exception ex)
            {
                return StatusCode(500, $"Internal server error: {ex.Message}");
            }
        }
    }


    public class WordleClient
    {
        private static readonly HttpClient httpClient = new HttpClient();

        public static async Task<string> GetCurrentAnswerWordAsync()
        {
            string host = "https://www.nytimes.com";
            string target = "";

            try
            {
                string dateString = DateTime.Now.ToString("yyyy-MM-dd");

                target = $"/svc/wordle/v2/{dateString}.json";
                string url = host + target;

                string jsonResponse = await httpClient.GetStringAsync(url);

                JObject response = JObject.Parse(jsonResponse);
                string solution = response["solution"]?.ToString().ToUpperInvariant();

                if (string.IsNullOrEmpty(solution))
                {
                    Console.WriteLine($"Error: 'solution' key not found in JSON response from {url}");
                    return null;
                }

                return solution;
            }
            catch (HttpRequestException ex)
            {
                Console.WriteLine($"Network error fetching Wordle answer from {host}{target}: {ex.Message}");
                return null;
            }
            catch (Newtonsoft.Json.JsonException ex)
            {
                Console.WriteLine($"Failed to parse JSON response: {ex.Message}");
                return null;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An unexpected error occurred: {ex.Message}");
                return null;
            }
        }
    }
}