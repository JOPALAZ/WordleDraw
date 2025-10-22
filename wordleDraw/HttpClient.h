#pragma once

#include <iostream>
#include <string>
#include <httplib.h>
#include <fmt/format.h>

using std::string;

class HttpClient
{
protected:

public:
    HttpClient();
    string fetch(string host, string endpoint);
};

