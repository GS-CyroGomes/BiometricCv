#include "StreamServer.hpp"

StreamServer::StreamServer(short port)
    : m_acceptor(m_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      m_socket(m_io_context) {
    Logger::getInstance().info("StreamServer inicializado na porta " + std::to_string(port));
}

StreamServer::~StreamServer() {
    stop();
}

void StreamServer::start() {
    if (!m_running) {
        m_running = true;
        do_accept();
        m_service_thread = std::thread([this]() { m_io_context.run(); });
        Logger::getInstance().info("StreamServer thread iniciada.");
    }
}

void StreamServer::stop() {
    if (m_running) {
        m_running = false;
        if (!m_io_context.stopped()) {
            m_io_context.stop();
        }
        if (m_service_thread.joinable()) {
            m_service_thread.join();
        }
        Logger::getInstance().info("StreamServer parado.");
    }
}

void StreamServer::do_accept() {
    m_acceptor.async_accept(m_socket,
        [this](const boost::system::error_code& ec) {
            handle_accept(ec);
        });
}

void StreamServer::handle_accept(const boost::system::error_code& error) {
    if (!error) {
        Logger::getInstance().info("Cliente conectado ao StreamServer.");
    } else {
        Logger::getInstance().error("Erro na aceitacao do StreamServer: " + error.message());
    }

    if (m_running) {
        do_accept();
    }
}

void StreamServer::streamFrame(const cv::Mat& frame) {
    if (!m_running || !m_socket.is_open() || frame.empty()) {
        return;
    }

    std::vector<uchar> buffer_local;
    cv::imencode(".jpg", frame, buffer_local, {cv::IMWRITE_JPEG_QUALITY, 80});

    m_io_context.post([this, buffer = std::move(buffer_local)]() mutable {
        async_send_frame(std::move(buffer));
    });
}

void StreamServer::async_send_frame(std::vector<uchar> buffer) {
    if (!m_socket.is_open()) return;

    uint32_t imageSize = buffer.size();

    boost::asio::async_write(m_socket, boost::asio::buffer(&imageSize, sizeof(imageSize)),
        [this, buffer = std::move(buffer)](const boost::system::error_code& ec, size_t /*length*/) {
            if (!ec) {
                boost::asio::async_write(m_socket, boost::asio::buffer(buffer),
                    [this](const boost::system::error_code& ec, size_t /*length*/) {
                        if (ec) {
                            Logger::getInstance().error("Erro ao enviar dados do frame no StreamServer: " + ec.message());
                            m_socket.close();
                        }
                    });
            } else {
                Logger::getInstance().error("Erro ao enviar tamanho do frame no StreamServer: " + ec.message());
                m_socket.close();
            }
        });
}
