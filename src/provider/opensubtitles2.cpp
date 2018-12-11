#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <xmlrpc/types.hpp>
#include <xmlrpc/rapidxml.hpp>

#include <memory>

#include <iostream>  // FIXME: debug

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>


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

constexpr auto REST_URL_BASE = "rest.opensubtitles.org";
constexpr auto XMLRPC_URL_BASE = "api.opensubtitles.org";
// FIXME: update and move to main program
static const auto USER_AGENT = UserAgent{"subdl2", "0.1.0"};

void
fail(boost::system::error_code ec, char const* what)
{
    std::cerr <<"FAIL: " << what << ": " << ec.message() << "\n";
}

class os_session : public::std::enable_shared_from_this<os_session> {
    tcp::resolver resolver_;
    ssl::stream<tcp::socket> stream_;
    boost::beast::flat_buffer buffer_; // (Must persist between reads)
    http::request<http::empty_body> req_;
    http::response<http::string_body> res_;
public:
    os_session(boost::asio::io_context& ioc, ssl::context& ctx)
    : resolver_(ioc)
    , stream_(ioc, ctx) {
    }
    void run(char const* host, char const* port, char const* target, int version) {
        if (!SSL_set_tlsext_host_name(stream_.native_handle(), host)) {
            boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            std::cerr << ec.message() << "\n";
            return;
        }
        req_.version(version);
        req_.method(http::verb::get);
        req_.target(target);
        req_.set(http::field::host, host);
        req_.set(http::field::user_agent, USER_AGENT.string());
        req_.keep_alive(false);

        resolver_.async_resolve(host, port,
                                [obj=shared_from_this()] (auto &&arg1, auto &&arg2) {
            obj->on_resolve(arg1, arg2);
        });
    }
    void on_resolve(boost::system::error_code ec, tcp::resolver::results_type results) {
        if(ec)
            return fail(ec, "resolve");

        // Make the connection on the IP address we get from a lookup
        boost::asio::async_connect(stream_.next_layer(), results.begin(), results.end(),
                    [obj=shared_from_this()](auto &&arg1, auto &&arg2) {
            obj->on_connect(arg1);
        });
    }
    void on_connect(boost::system::error_code ec) {
        if(ec)
            return fail(ec, "connect");

        // Perform the SSL handshake
        stream_.async_handshake(ssl::stream_base::client, [obj=shared_from_this()](auto &&arg1) {
            obj->on_handshake(arg1);
        });
    }

    void on_handshake(boost::system::error_code ec) {
        if(ec)
            return fail(ec, "handshake");


        std::cerr << "writing " << req_ << '\n';
        // Send the HTTP request to the remote host
        http::async_write(stream_, req_, [obj=shared_from_this() ](auto &&arg1, auto &&arg2) {
            obj->on_write(arg1, arg2);
        });
    }

    void on_write(boost::system::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");

        // Receive the HTTP response
        http::async_read(stream_, buffer_, res_,
                         [obj=shared_from_this()](auto &&arg1, auto &&arg2) {
            obj->on_read(arg1, arg2);
        });
    }

    void on_read(boost::system::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "read");

        // Write the message to standard out
        std::cout << res_ << std::endl;

        // Gracefully close the stream
        stream_.async_shutdown([obj=shared_from_this()](auto &&arg) {
            obj->on_shutdown(arg);
        });
    }
    void on_shutdown(boost::system::error_code ec)
    {
        if(ec == boost::asio::error::eof)
        {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec.assign(0, ec.category());
        }
        if(ec)
            return fail(ec, "shutdown");

        // If we get here then the connection is closed gracefully
    }
};


void do_opensubtitles_boost_beast() {
    boost::asio::io_context ioc;

    ssl::context ctx{ssl::context::sslv23_client};


    auto host = XMLRPC_URL_BASE;
    auto version = 11;
    auto port = "443";
    auto target = "/xml-rpc";


    std::make_shared<os_session>(ioc, ctx)->run(host, port, target, version);

    ioc.run();
}

}
}
}
