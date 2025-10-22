#include "WordleRestClient.h"

WordleRestClient::WordleRestClient()
{

}

string WordleRestClient::GetCurrentAnswerWord()
{
//https://www.nytimes.com/svc/wordle/v2/2025-10-22.json


	auto timeNow = std::chrono::system_clock::now();
	auto const zone = std::chrono::current_zone();
	auto const localTime = std::chrono::zoned_time(zone, timeNow);
	string host = "https://www.nytimes.com";
	string target = fmt::format("/svc/wordle/v2/{}.json", std::format("{:%Y-%m-%d}", localTime));
	auto response = nlohmann::json::parse(client.fetch(host, target));
	return response["solution"];
}
