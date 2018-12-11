
#include <cpprest/http_client.h>
#include <cpprest/json.h>

//FIXME: try pistache once it supports SSL
//#include <pistache/net.h>
//#include <pistache/http.h>
//#include <pistache/client.h>

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
constexpr auto XMLRPC_URL_BASE = "https://api.opensubtitles.org";
// FIXME: update and move to main program
static const auto USER_AGENT = UserAgent{"subdl2", "0.1.0"};

}
}
}

using namespace subdl::provider::opensubtitles;

void do_opensubtitles_cpprestsdk() {
    web::http::client::http_client client{REST_URL_BASE};
    web::uri_builder builder("/search");
    builder.append_path("moviebytesize-750005572");
    builder.append_path("moviehash-319b23c54e9cf314");
    std::cout << "uri: " << builder.to_uri().to_string() << "\n";
    web::http::http_request request{};
    request.set_request_uri(builder.to_uri());
    request.headers().add("User-Agent", USER_AGENT.string());

    auto task = client.request(request).then([=](web::http::http_response response) {
        if (response.status_code() != web::http::status_codes::OK) {

        }
        std::cout << "Received response status code: " << response.status_code() << '\n';
        for (const auto &header : response.headers()) {
            std::cout << header.first << ": " << header.second << '\n';
        }
        const auto &headers = response.headers();
        auto it = headers.find("Set-Cookie");
        if (it != headers.end()) {
            std::cout << "found set-cookie: " << it->second << "\n";
        }
        std::cout << "am here now\n";
        std::cout.flush();

        return response.extract_json(true);
    }).then([](web::json::value json) {
        std::cout << "array: " << json.is_array() << '\n';
        if (!json.is_array()) {
            std::cerr << "expected an array...\n";
            return;
        }
        const auto &array = json.as_array();
        for(const auto &item : array) {
            item.serialize(std::cout);
            std::cout << '\n';
        }
        //json.serialize(std::cout);
    });

    task.wait();
//    client.request(request, )
}

#if 0
#include <libiqxmlrpc/libiqxmlrpc.h>
#include <libiqxmlrpc/client.h>
#include <libiqxmlrpc/https_client.h>
#include <libiqxmlrpc/inet_addr.h>
void opensubtitles_iqxmlrpc() {
    iqnet::ssl::ctx = iqnet::ssl::Ctx::client_only();
    iqxmlrpc::Client<iqxmlrpc::Https_client_connection> client(iqnet::Inet_addr("api.opensubtitles.org", 443), "/xml-rpc");

    iqxmlrpc::Param_list pl;
    pl.push_back(""); // username
    pl.push_back(""); // password
    pl.push_back("en"); // language
    pl.push_back(USER_AGENT.string()); // useragent

    iqxmlrpc::Response r = client.execute("LogIn", pl);
    std::cout << "response: fault=" << r.is_fault() << ", code=" << r.fault_code() << '\n';
    //std::cout << "can_cast_string=" << r.can_cast<std::string>() << '\n';
    std::cout << "rseponse: " << r.value().get_string() << '\n';
    // pl.push_back("username", "");
    // pl.push_back("password", "");
    // pl.push_back("language", "en");

}
#endif

#include <rapidxml.h>
#include <rapidxml_print.h>

void opensubtitles_cpprestsdk_xmlprc() {
    web::http::client::http_client client{XMLRPC_URL_BASE};
    web::uri_builder builder("/xml-rpc");
    web::http::http_request request{};
    request.set_request_uri(builder.to_uri());

    rapidxml::xml_document document{};
    document.allocate_node(rapidxml::node_document);
    //auto document = pool.allocate_node(rapidxml::node_document);
    auto methodCall = document.allocate_node(rapidxml::node_element, "methodCall");
    document.append_node(methodCall);
    methodCall->append_node(document.allocate_node(rapidxml::node_element, "methodName", "LogIn"));
    auto params_node = document.allocate_node(rapidxml::node_element, "params");
    methodCall->append_node(params_node);
    {
        auto value_node = document.allocate_node(rapidxml::node_element, "value");
        params_node->append_node(value_node);
    }
    std::cout << document << "\n";
    std::cout.flush();

    //request.headers().add("User-Agent", USER_AGENT.string());

    auto task = client.request(request).then([=](web::http::http_response response) {
        if (response.status_code() != web::http::status_codes::OK) {
            std::cout << "response = OK\n";
        }
        std::cout << "Received response status code: " << response.status_code() << '\n';
        for (const auto &header : response.headers()) {
            std::cout << header.first << ' ' << header.second << '\n';
        }
        const auto &headers = response.headers();
        auto it = headers.find("Set-Cookie");
        if (it != headers.end()) {
            std::cout << "found set-cookie: " << it->second << "\n";
        }

        return response.extract_string();
    }).then([](std::string text) {
        std::cout << "response text: " << text << '\n';
        rapidxml::xml_document<> doc;
        doc.parse<0>(text.data());
        std::cout << doc;
        rapidxml::print(std::cout, doc, 2);
    });

    task.wait();
}
