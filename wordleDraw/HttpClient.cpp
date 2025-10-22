#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "HttpClient.h"

HttpClient::HttpClient()
{
}

string HttpClient::fetch(string host, string endpoint)
{
    httplib::Client cli(host);

    cli.set_connection_timeout(5, 0);

    if (auto res = cli.Get(endpoint)) {

        if (res->status == 200) {
            return res->body;
        }
        else {
            throw std::runtime_error(fmt::format("Error during request to [{}], at endpoint [{}]: Status: {}, {}", host, endpoint, res->status, res->body));
        }

    }
    else {
        throw std::runtime_error(fmt::format("Error during request to [{}], at endpoint [{}]: Status: {}, {}", host, endpoint, std::to_string(res->status), res->body));
    }
}
