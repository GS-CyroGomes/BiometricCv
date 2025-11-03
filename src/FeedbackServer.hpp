#ifndef FEEDBACK_SERVER_HPP
#define FEEDBACK_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <thread>
#include <string>
#include <deque>
#include <mutex>
#include <atomic>

#include "Logger.hpp"

class FeedbackServer {
public:
    FeedbackServer(short port);
    ~FeedbackServer();

    void start();
    void stop();
    void sendFeedback(const std::string& message);

private:
    void run_service();
    void do_accept();
    void handle_accept(const boost::system::error_code& error);
    void do_write();
    void handle_write(const boost::system::error_code& error, size_t bytes_transferred);

    boost::asio::io_context m_io_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
    std::thread m_service_thread;
    std::atomic<bool> m_running = false;
    
    std::deque<std::string> m_messageQueue;
    std::mutex m_queueMutex;
    bool m_writing = false;
};

#endif // FEEDBACK_SERVER_HPP
