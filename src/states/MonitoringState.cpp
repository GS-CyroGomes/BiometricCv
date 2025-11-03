#include "MonitoringState.hpp"
#include "../StateController.hpp"
#include "../Config.hpp"
#include "../Logger.hpp"

MonitoringState::MonitoringState(StateController& controller)
    : IAppState(controller) {}

void MonitoringState::onEnter() {
    Logger::getInstance().info("STATE: Entrando em 'Monitoramento'");
    m_lastFrameTime = std::chrono::steady_clock::now();
    m_currentFPS = 0.0;
    m_fpsMax = 0.0;
    m_fpsMin = 9999.0;
}

void MonitoringState::onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) {
    auto now = std::chrono::steady_clock::now();
    double elapsedTime = std::chrono::duration<double>(now - m_lastFrameTime).count();
    m_lastFrameTime = now;

    if (elapsedTime > 0) {
        m_currentFPS = 1.0 / elapsedTime;
        if (m_currentFPS > 8) {
            m_fpsMin = std::min(m_fpsMin, m_currentFPS);
            m_fpsMax = std::max(m_fpsMax, m_currentFPS);
        }
    }

    std::string faceStatus;
    if (!analysisResult.faceFound) {
        faceStatus = "NENHUMA FACE DETECTADA";
    } else {
        faceStatus = "FACE DETECTADA";
        cv::rectangle(frame, analysisResult.faceRect, Config::COLOR_WHITE, 2);
    }

    std::string displayText = "MONITORAMENTO ATIVO\n"
                    "FPS: " + std::to_string(static_cast<int>(m_currentFPS)) + 
                    " (Min: " + std::to_string(static_cast<int>(m_fpsMin)) + 
                    ", Max: " + std::to_string(static_cast<int>(m_fpsMax)) + ")\n" +
                    "Faces: " + faceStatus;
    
    uiManager.drawCenteredText(frame, displayText);
}

void MonitoringState::onExit() {
    Logger::getInstance().info("STATE: Saindo de 'Monitoramento'");
}