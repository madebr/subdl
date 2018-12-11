const char *responseText =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<methodResponse>"
            "<params>"
                "<param>"
                    "<value>"
                        "<struct>"
                            "<member>"
                                "<name>token</name>"
                                "<value><string>GO9Loc0-yVq6IWeavxdarwCjCD6</string></value>"
                            "</member>"
                            "<member>"
                                "<name>status</name>"
                                "<value><string>200 OK</string></value>"
                            "</member>"
                            "<member>"
                                "<name>seconds</name>"
                                "<value><double>0.003</double></value>"
                            "</member>"
                        "</struct>"
                    "</value>"
                "</param>"
            "</params>"
        "</methodResponse>";

#include <xmlrpc/rapidxml.hpp>

#include "rapidxml.h"
#include "rapidxml_print.h"

#include <stdexcept>
#include <string>

#include <iostream>

using TokenType = std::string;

enum class StatusCode : std::uint16_t{
    OK = 200,
    PartialContent = 206,

    Unauthorized = 401,
    SubtitleInvalidFormat = 402,
    SubHashesNotSame = 403,
    SubtitleInvalidLanguage = 404,
    MandatoryParametersMissing = 405,
    NoSession = 406,
    DownloadLimitReached = 407,
    InvalidParametes = 408,
    MethodNotFound = 409,
    UnknownError = 410,
    InvalidUseragent = 411,
    InvalidFormat = 412,

    TemporaryDown = 501,
    ServiceUnavailable = 503,
    Moved = 301,

};


struct LogInResponse {
    TokenType token;
    double time;
};

class SessionError : std::exception {
    StatusCode code_;
    std::string message_;

};

#include <cassert>
#include <cstring>

void parse_login_text(std::string &text) {
    std::cout.flush();
    rapidxml::xml_document<> document;
    document.parse<rapidxml::parse_fastest>(text.data());
    auto methodResponseNode = document.first_node("methodResponse");
    if (methodResponseNode == nullptr) {
        std::cerr << "no methodResponse\n";
        return;
    }
    auto paramsNode = methodResponseNode->first_node("params");
    if (paramsNode == nullptr) {
        std::cerr << "no params\n";
        return;
    }
    auto paramNode = paramsNode->first_node("param");
    if (paramNode == nullptr) {
        std::cerr << "no param\n";
        return;
    }
    auto valueNode = paramNode->first_node("value");
    if (valueNode == nullptr) {
        std::cerr << "no value\n";
        return;
    }
    auto node = xmlrpc::rapidxml::XmlToValueConverter<>::extract_value(valueNode->first_node());
    if (!xmlrpc::holds_alternative<xmlrpc::Struct>(node)) {
        std::cerr << "expected struct\n";
        return;
    }
    auto s = std::move(xmlrpc::get<xmlrpc::Struct>(node));\
    std::cerr << "size: " << s.size() << '\n';
    auto time = s.get<xmlrpc::Double>("seconds").value();
    auto status = s.get<xmlrpc::String>("status").value();
    auto token = s.get<xmlrpc::String>("token").value();
    std::cerr << "time: " << time << "s, status: " << status << ", token: " << token << '\n';
//    auto map = std::move(xmlrpc::get<xmlrpc::Struct>(node).value());
}


void test() {
    {
        std::string txt(responseText);

        rapidxml::xml_document<> document;
        document.parse<0>(txt.data());
        txt.resize(10000, 'Z');
        std::cout << document;
    }
    {
        std::string txt(responseText);
        parse_login_text(txt);
    }
}

void main_test_build() {
    test();
}
