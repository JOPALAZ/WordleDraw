
using System.Linq;
using Microsoft.AspNetCore.Mvc;
using WordleDrawBackendWebApi.Models;

namespace WordleDrawBackendWebApi.Controllers
{
    [ApiController]
    [Route("/calculateWordleAnswer")]
    public class WordleController : ControllerBase
    {
        IWordleDrawCoreWrapper wrapper;
        public WordleController(IWordleDrawCoreWrapper wordleDrawCoreWrapper) 
        {
            wrapper = wordleDrawCoreWrapper;
        }

        [HttpPost]
        public IActionResult calculateWordleAnswer([FromBody] DrawOptions requestModel) 
        {
            try
            {
                var result = wrapper.GetWordsSolution(requestModel);
                return Ok(new { Response = string.Join('\n', result) });
            }
            catch (NoSolutionException ex)
            {
                return BadRequest(new { Response = ex.Message });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { Response = ex.Message });
            }
        }

        [HttpPost("/colors")]
        public IActionResult getColors([FromBody] ColorRequest requestModel) 
        {
            try
            {
                var result = wrapper.GetColors(requestModel);
                return Ok(new { Response = result });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { Response = ex.Message });
            }
        }
    }
}
