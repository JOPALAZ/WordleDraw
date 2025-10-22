#pragma once
#include <iostream>
#include "HttpClient.h"
#include <chrono>
#include <nlohmann/json.hpp>
using std::string;
class WordleRestClient
{
public:
	WordleRestClient();
	string GetCurrentAnswerWord();
protected:
	HttpClient client;
};

