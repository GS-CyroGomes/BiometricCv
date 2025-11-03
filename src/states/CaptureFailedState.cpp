#include "CaptureFailedState.hpp"
#include "SearchingFaceState.hpp"
#include "../StateController.hpp"
#include "../Logger.hpp"

CaptureFailedState::CaptureFailedState(StateController& controller, const std::string& errorMessage)
    : IAppState(controller), m_errorMessage(errorMessage) {
        m_message = "Captura Falhou: " + m_errorMessage;
    }

void CaptureFailedState::onEnter() {
    Logger::getInstance().error("STATE: Entrando em 'Captura Falhou': " + m_errorMessage);
    m_displayStartTime = std::chrono::steady_clock::now();
}

void CaptureFailedState::onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) {
    uiManager.drawCenteredText(frame, m_message);

    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - m_displayStartTime).count() > Config::POST_CAPTURE_DISPLAY_SECONDS) {
        m_controller.transitionTo(std::make_unique<SearchingFaceState>(m_controller)); // Volta para buscar novo rosto
    }
}

void CaptureFailedState::onExit() {
    Logger::getInstance().info("STATE: Saindo de 'Captura Falhou'");
}
