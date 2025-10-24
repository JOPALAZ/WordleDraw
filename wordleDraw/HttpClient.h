#pragma once

#include <iostream>
#include <string>
#include <httplib.h>
#include <fmt/format.h>

class HttpClient
{
protected:
    int SafeGetStatus(httplib::Result& res);
    std::string SafeGetBody(httplib::Result& res);
public:
    HttpClient();
    std::string fetch(std::string host, std::string endpoint);
};

