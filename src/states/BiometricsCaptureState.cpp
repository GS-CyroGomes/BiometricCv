#include "BiometricsCaptureState.hpp"
#include "SearchingFaceState.hpp"
#include "CaptureSuccessState.hpp"
#include "CaptureFailedState.hpp"
#include "../StateController.hpp"
#include "../Logger.hpp"

BiometricsCaptureState::BiometricsCaptureState(StateController& controller)
    : IAppState(controller), m_message("Iniciando captura...") {}

void BiometricsCaptureState::onEnter() {
    Logger::getInstance().info("STATE: Entrando em 'Captura Biometrica'");
    m_currentPhase = GUIDING;
    m_captureTriggered = false;
    m_controller.resetCaptureRequest(); // Consome o pedido de captura
}

void BiometricsCaptureState::onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) {
    // Se o rosto for perdido, volta para o estado de busca
    if (!analysisResult.faceFound) {
        m_message = "Rosto perdido! Buscando...";
        uiManager.drawCenteredText(frame, m_message);
        m_controller.transitionTo(std::make_unique<SearchingFaceState>(m_controller));
        return;
    }

    // Desenha a elipse guia e os landmarks
    cv::Scalar feedbackColor = analysisResult.isCompliant ? Config::COLOR_GREEN : Config::COLOR_RED;
    cv::Point center(frame.cols / 2, frame.rows / 2);
    cv::Size axes(static_cast<int>(frame.cols / 3.5), static_cast<int>(frame.rows / 2.2));
    uiManager.drawEllipse(frame, center, axes, feedbackColor);
    if (!analysisResult.landmarks.empty()) {
        for (const auto& point : analysisResult.landmarks[0]) {
            cv::circle(frame, point, 2, Config::COLOR_BLUE, -1);
        }
    }

    // Lógica das fases de captura
    switch (m_currentPhase) {
        case GUIDING:
            m_message = analysisResult.failureReason;
            if (analysisResult.isCompliant) {
                m_currentPhase = COUNTDOWN;
                m_phaseStartTime = std::chrono::steady_clock::now();
                m_countdownValue = Config::CAPTURE_COUNTDOWN_SECONDS;
                Logger::getInstance().info("INFO: Rosto em conformidade. Iniciando contagem regressiva...");
            }
            break;

        case COUNTDOWN:
            {
                auto now = std::chrono::steady_clock::now();
                int elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - m_phaseStartTime).count();
                m_countdownValue = Config::CAPTURE_COUNTDOWN_SECONDS - elapsedSeconds;

                if (m_countdownValue > 0) {
                    m_message = "Captura em " + std::to_string(m_countdownValue) + "...";
                } else {
                    m_currentPhase = CAPTURING;
                    m_message = "Capturando...";
                    Logger::getInstance().info("INFO: Contagem regressiva finalizada. Realizando captura...");
                }
            }
            break;

        case CAPTURING:
            if (!m_captureTriggered) {
                saveImage(frame, analysisResult.faceRect); // Passa o faceRect
                m_captureTriggered = true;
                m_currentPhase = POST_CAPTURE;
            }
            break;

        case POST_CAPTURE:
            // A transição é feita no método saveImage
            break;
    }
    uiManager.drawCenteredText(frame, m_message);
}

void BiometricsCaptureState::onExit() {
    Logger::getInstance().info("STATE: Saindo de 'Captura Biometrica'");
}

void BiometricsCaptureState::saveImage(const cv::Mat& frame, const cv::Rect& faceRect) {
    // A lógica de salvamento real foi movida para CaptureSuccessState
    // Apenas transicionamos para o estado de sucesso, passando os dados necessários
    m_controller.transitionTo(std::make_unique<CaptureSuccessState>(m_controller, frame, faceRect));
}
