#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include <string>
#include <queue>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include <functional>
#include <thread>

class SocketServer {
public:
    SocketServer(short port, std::function<void(const std::string&)> command_handler);
    ~SocketServer();

    void start();
    void stop();
    void sendResponse(const std::string& response);

private:
    void run_service();
    void do_accept();
    void handle_accept(const boost::system::error_code& error);
    void do_read();
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

    boost::asio::io_context m_io_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
    std::thread m_service_thread;
    std::function<void(const std::string&)> m_command_handler;
};

#endif // SOCKET_SERVER_HPP