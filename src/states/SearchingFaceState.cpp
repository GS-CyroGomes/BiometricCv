#include "SearchingFaceState.hpp"
#include "ValidatingState.hpp"
#include "BiometricsCaptureState.hpp"
#include "../StateController.hpp"
#include "../Logger.hpp"

SearchingFaceState::SearchingFaceState(StateController& controller)
    : IAppState(controller), m_message("Aproxime-se e centralize o rosto") {}

void SearchingFaceState::onEnter() {
    Logger::getInstance().info("STATE: Entrando em 'Buscando Rosto'");
}

void SearchingFaceState::onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) {
    m_message = "Aproxime-se e centralize o rosto";
    uiManager.drawCenteredText(frame, m_message);

    // Se o FaceProcessor encontrou um rosto, mudamos de estado
    if (analysisResult.faceFound) {
        m_controller.transitionTo(std::make_unique<ValidatingState>(m_controller));
    }
}

void SearchingFaceState::onExit() {
    Logger::getInstance().info("STATE: Saindo de 'Buscando Rosto'");
}
