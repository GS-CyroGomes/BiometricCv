#include "VideoRecorder.hpp"

VideoRecorder::VideoRecorder() {
    // Construtor
}

VideoRecorder::~VideoRecorder() {
    stopRecording(); // Garante que o gravador seja parado ao destruir o objeto
}

bool VideoRecorder::startRecording(const std::string& filename, int frameWidth, int frameHeight, double fps) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_isRecording) {
        Logger::getInstance().warning("Gravacao de video ja esta ativa.");
        return false;
    }

    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G'); 

    m_videoWriter.open(filename, fourcc, fps, cv::Size(frameWidth, frameHeight));

    if (!m_videoWriter.isOpened()) {
        Logger::getInstance().error("Falha ao iniciar gravacao de video para: " + filename);
        return false;
    }

    m_isRecording = true;
    m_startTime = std::chrono::high_resolution_clock::now();
    Logger::getInstance().info("Gravacao de video iniciada: " + filename);
    return true;
}

void VideoRecorder::stopRecording() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_isRecording) {
        m_videoWriter.release();
        m_isRecording = false;
        Logger::getInstance().info("Gravacao de video parada.");
    }
}

void VideoRecorder::writeFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_isRecording && !frame.empty()) {
        m_videoWriter.write(frame);
    }
}

bool VideoRecorder::isRecording() const {
    return m_isRecording;
}

double VideoRecorder::getRecordedDuration() const {
    if (m_isRecording) {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - m_startTime).count();
    }
    return 0.0;
}
