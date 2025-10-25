using WordleDrawBackendWebApi;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddSingleton<IWordleDrawCoreWrapper>(new WordleDrawCoreWrapper("valid-wordle-words.txt"));
builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseHttpsRedirection();
app.UseAuthorization();

app.UseDefaultFiles();
app.UseStaticFiles();

app.MapControllers();
app.Run();
