#include "FeedbackServer.hpp"

FeedbackServer::FeedbackServer(short port)
    : m_acceptor(m_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      m_socket(m_io_context) {
    Logger::getInstance().info("FeedbackServer inicializado na porta " + std::to_string(port));
}

FeedbackServer::~FeedbackServer() {
    stop();
}

void FeedbackServer::start() {
    if (!m_running) {
        m_running = true;
        do_accept();
        m_service_thread = std::thread([this]() { m_io_context.run(); });
        Logger::getInstance().info("FeedbackServer thread iniciada.");
    }
}

void FeedbackServer::stop() {
    if (m_running) {
        m_running = false;
        if (!m_io_context.stopped()) {
            m_io_context.stop();
        }
        if (m_service_thread.joinable()) {
            m_service_thread.join();
        }
        Logger::getInstance().info("FeedbackServer parado.");
    }
}

void FeedbackServer::do_accept() {
    m_acceptor.async_accept(m_socket,
        [this](const boost::system::error_code& ec) {
            handle_accept(ec);
        });
}

void FeedbackServer::handle_accept(const boost::system::error_code& error) {
    if (!error) {
        Logger::getInstance().info("Cliente conectado ao FeedbackServer.");
        do_write(); // Start writing if there are pending messages
    } else {
        Logger::getInstance().error("Erro na aceitacao do FeedbackServer: " + error.message());
    }

    if (m_running) {
        do_accept();
    }
}

void FeedbackServer::sendFeedback(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    m_messageQueue.push_back(message + "\n");
    if (!m_writing) {
        m_io_context.post([this]() { do_write(); });
    }
}

void FeedbackServer::do_write() {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    if (m_messageQueue.empty() || !m_socket.is_open()) {
        m_writing = false;
        return;
    }
    m_writing = true;

    boost::asio::async_write(m_socket, boost::asio::buffer(m_messageQueue.front()),
        [this](const boost::system::error_code& ec, size_t length) {
            handle_write(ec, length);
        });
}

void FeedbackServer::handle_write(const boost::system::error_code& error, size_t bytes_transferred) {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    if (!error) {
        m_messageQueue.pop_front();
        if (!m_messageQueue.empty()) {
            m_io_context.post([this]() { do_write(); });
        } else {
            m_writing = false;
        }
    } else {
        Logger::getInstance().error("Erro ao enviar feedback: " + error.message());
        m_socket.close();
        m_writing = false;
    }
}
