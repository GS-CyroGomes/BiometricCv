#include "StateController.hpp"
#include "states/SearchingFaceState.hpp"
#include "Logger.hpp"

StateController::StateController() {
    // Inicia a aplicação no estado "Buscando Rosto"
    m_currentState = std::make_unique<SearchingFaceState>(*this);
    m_currentState->onEnter();
}

void StateController::transitionTo(std::unique_ptr<IAppState> newState) {
    if (m_currentState) {
        m_currentState->onExit();
    }
    m_currentState = std::move(newState);
    if (m_currentState) {
        m_currentState->onEnter();
    }
}

void StateController::update(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) {
    if (m_currentState) {
        m_currentState->onUpdate(analysisResult, frame, uiManager);
    }
}

std::string StateController::getCurrentStateMessage() const {
    if (m_currentState) {
        return m_currentState->getMessage();
    }
    return "Nenhum estado";
}

void StateController::requestCapture() {
    m_captureRequested = true;
    m_isBiometrics = false;
    Logger::getInstance().info("Captura padrão solicitada externamente.");
}

void StateController::requestBiometricsCapture() {
    m_captureRequested = true;
    m_isBiometrics = true;
    Logger::getInstance().info("Captura biométrica solicitada externamente.");
}
