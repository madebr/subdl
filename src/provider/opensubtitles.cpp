
#include <cpprest/http_client.h>

#include <sstream>
#include <string>

#include <iostream>

namespace subdl {
namespace provider {
namespace opensubtitles {

struct UserAgent {
    std::string name;
    std::string version;
    std::string string() const {
        std::ostringstream oss;
        oss << name << ' ' << version;
        return oss.str();
    }
};

constexpr auto REST_URL_BASE = "https://rest.opensubtitles.org";
// FIXME: update and move to main program
const auto USER_AGENT = UserAgent{"SubDownloader", "2.1.0"};

}
}
}


using namespace subdl::provider::opensubtitles;

void do_opensubtitles() {
    web::http::client::http_client client{REST_URL_BASE};
    web::uri_builder builder("/search");
    builder.append_path("moviebytesize-750005572");
    builder.append_path("moviehash-319b23c54e9cf314");
    std::cout << "uri: " << builder.to_uri().to_string() << "\n";
    web::http::http_request request{};
    request.set_request_uri(builder.to_uri());
    request.headers().add("User-Agent", USER_AGENT.string());

    auto task = client.request(request).then([=](web::http::http_response response) {
        std::cout << "Received response status code: " << response.status_code() << '\n';
        for (const auto &header : response.headers()) {
            std::cout << header.first << ' ' << header.second << '\n';
        }
        const auto &headers = response.headers();
        auto it = headers.find("Set-Cookie");
        if (it != headers.end()) {
            std::cout << "found set-cookie: " << it->second << "\n";
        }
//        return response.
    });

    task.wait();
//    client.request(request, )
}
