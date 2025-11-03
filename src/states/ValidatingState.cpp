#include "ValidatingState.hpp"
#include "SearchingFaceState.hpp"
#include "BiometricsCaptureState.hpp"
#include "../StateController.hpp"
#include "../Logger.hpp"

#include "BiometricsCaptureState.hpp"

ValidatingState::ValidatingState(StateController& controller)
    : IAppState(controller), m_message("Validando...") {}

void ValidatingState::onEnter() {
    Logger::getInstance().info("STATE: Entrando em 'Validando Rosto'");
}

void ValidatingState::onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) {
    // Se perdemos o rosto, voltamos para o estado de busca
    if (!analysisResult.faceFound) {
        m_controller.transitionTo(std::make_unique<SearchingFaceState>(m_controller));
        return;
    }

    m_message = analysisResult.failureReason;

    // Determina a cor do feedback e desenha a elipse
    cv::Scalar feedbackColor = analysisResult.isCompliant ? Config::COLOR_GREEN : Config::COLOR_RED;
    cv::Point center(frame.cols / 2, frame.rows / 2);
    cv::Size axes(static_cast<int>(frame.cols / 3.5), static_cast<int>(frame.rows / 2.2));
    uiManager.drawEllipse(frame, center, axes, feedbackColor);
    uiManager.drawCenteredText(frame, m_message);

    // Desenha os landmarks para depuração
    if (!analysisResult.landmarks.empty()) {
        for (const auto& point : analysisResult.landmarks[0]) {
            cv::circle(frame, point, 2, Config::COLOR_BLUE, -1);
        }
    }

    // Se todas as validações passaram E a captura foi solicitada, transiciona para o estado de captura
    if (analysisResult.isCompliant && m_controller.isCaptureRequested()) {
        m_controller.transitionTo(std::make_unique<BiometricsCaptureState>(m_controller));
    }
}

void ValidatingState::onExit() {
    Logger::getInstance().info("STATE: Saindo de 'Validando Rosto'");
}
