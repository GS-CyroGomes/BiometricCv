#ifndef STREAM_SERVER_HPP
#define STREAM_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>
#include <mutex>

#include "Logger.hpp"

class StreamServer {
public:
    StreamServer(short port);
    ~StreamServer();

    void start();
    void stop();
    void streamFrame(const cv::Mat& frame);

private:
    void run_service();
    void do_accept();
    void handle_accept(const boost::system::error_code& error);
    void async_send_frame(std::vector<uchar> buffer);
    void handle_send_size(const boost::system::error_code& error, size_t bytes_transferred, std::vector<uchar> buffer);
    void handle_send_data(const boost::system::error_code& error, size_t bytes_transferred);

    boost::asio::io_context m_io_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
    std::thread m_service_thread;
    std::atomic<bool> m_running = false;
    std::mutex m_socketMutex;
};

#endif // STREAM_SERVER_HPP
