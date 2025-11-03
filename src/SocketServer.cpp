#include "SocketServer.hpp"
#include "Logger.hpp"

SocketServer::SocketServer(short port, std::function<void(const std::string&)> command_handler)
    : m_acceptor(m_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      m_socket(m_io_context),
      m_command_handler(command_handler) {
}

SocketServer::~SocketServer() {
    stop();
}

void SocketServer::start() {
    do_accept();
    m_service_thread = std::thread([this]() { m_io_context.run(); });
    Logger::getInstance().info("SocketServer iniciado na porta " + std::to_string(m_acceptor.local_endpoint().port()));
}

void SocketServer::stop() {
    if (!m_io_context.stopped()) {
        m_io_context.stop();
    }
    if (m_service_thread.joinable()) {
        m_service_thread.join();
    }
    Logger::getInstance().info("SocketServer parado.");
}

void SocketServer::sendResponse(const std::string& response) {
    if (m_socket.is_open()) {
        boost::asio::write(m_socket, boost::asio::buffer(response));
    }
}

void SocketServer::do_accept() {
    m_acceptor.async_accept(m_socket,
        [this](const boost::system::error_code& ec) {
            if (!ec) {
                Logger::getInstance().info("Cliente conectado ao SocketServer.");
                do_read();
            } else {
                Logger::getInstance().error("Erro na aceitacao do SocketServer: " + ec.message());
            }
        });
}

void SocketServer::do_read() {
    boost::asio::async_read_until(m_socket, m_buffer, "\n",
        [this](const boost::system::error_code& ec, std::size_t length) {
            if (!ec) {
                std::istream is(&m_buffer);
                std::string line;
                std::getline(is, line);
                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }
                
                if(m_command_handler) {
                    m_command_handler(line);
                }
                
                do_read(); // Continua lendo
            } else if (ec != boost::asio::error::eof) {
                Logger::getInstance().error("Erro de leitura no SocketServer: " + ec.message());
            }
        });
}
