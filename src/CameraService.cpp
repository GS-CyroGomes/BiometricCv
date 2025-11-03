#include "CameraService.hpp"
#include "Logger.hpp"

CameraService::CameraService(int cameraIndex) : m_cameraIndex(cameraIndex), m_width(0), m_height(0) {}

CameraService::~CameraService() {
    release();
}

bool CameraService::open() {
    #ifdef _WIN32
        m_capture.open(m_cameraIndex, cv::CAP_DSHOW);
    #else
        m_capture.open(m_cameraIndex);
    #endif
    
    if (!m_capture.isOpened()) {
        Logger::getInstance().error("Erro ao abrir a camera com indice " + std::to_string(m_cameraIndex));
        return false;
    }

    Logger::getInstance().info("Camera com indice " + std::to_string(m_cameraIndex) + " aberta com sucesso.");
    configureResolution();
    return true;
}

void CameraService::release() {
    if (m_capture.isOpened()) {
        m_capture.release();
        Logger::getInstance().info("Camera com indice " + std::to_string(m_cameraIndex) + " liberada.");
    }
}

bool CameraService::readFrame(cv::Mat& frame) {
    if (!m_capture.isOpened()) {
        Logger::getInstance().error("Camera nao esta aberta.");
        return false;
    }

    return m_capture.read(frame);
}

bool CameraService::isOpened() const {
    return m_capture.isOpened();
}

void CameraService::configureResolution() {
    m_capture.set(cv::CAP_PROP_FRAME_WIDTH, Config::MIN_IMAGE_WIDTH);
    m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, Config::MIN_IMAGE_HEIGHT);
    m_capture.set(cv::CAP_PROP_FPS, Config::CAPTURE_FPS);

    m_width = static_cast<int>(m_capture.get(cv::CAP_PROP_FRAME_WIDTH));
    m_height = static_cast<int>(m_capture.get(cv::CAP_PROP_FRAME_HEIGHT));

    Logger::getInstance().info("Resolucao da camera configurada para " + std::to_string(m_width) + "x" + std::to_string(m_height));

    if (m_width < Config::MIN_IMAGE_WIDTH || m_height < Config::MIN_IMAGE_HEIGHT) {
        Logger::getInstance().warning("Aviso: A resolucao da camera (" + std::to_string(m_width) + "x" + std::to_string(m_height) + ") e menor que a resolucao minima recomendada (" + std::to_string(Config::MIN_IMAGE_WIDTH) + "x" + std::to_string(Config::MIN_IMAGE_HEIGHT) + ").");
    }
}
