#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "HttpClient.h"

using std::string;
using std::runtime_error;

HttpClient::HttpClient()
{
}

string HttpClient::fetch(string host, string endpoint)
{
    httplib::Client cli(host);

    cli.set_connection_timeout(5, 0);

    if (httplib::Result res = cli.Get(endpoint)) {

        if (SafeGetStatus(res) == 200) {
            return SafeGetBody(res);
        }
        else {
            throw runtime_error(fmt::format("Error during request to [{}], at endpoint [{}]: Status: {}, {}", host, endpoint, std::to_string(SafeGetStatus(res)), SafeGetBody(res)));
        }

    }
    else {
        throw runtime_error(fmt::format("Error during request to [{}], at endpoint [{}]: Status: {}, {}", host, endpoint, std::to_string(res->status), SafeGetBody(res)));
    }
}

int HttpClient::SafeGetStatus(httplib::Result& res)
{
    return res.operator->() ? res->status : 0;
}

string HttpClient::SafeGetBody(httplib::Result& res)
{
    return res.operator->() ? res->body : "";
}