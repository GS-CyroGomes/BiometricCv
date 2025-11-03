#include "EnvironmentCheckState.hpp"
#include "SearchingFaceState.hpp"
#include "../StateController.hpp"
#include "../Config.hpp"
#include "../Logger.hpp"

EnvironmentCheckState::EnvironmentCheckState(StateController& controller)
    : IAppState(controller) {}

void EnvironmentCheckState::onEnter() {
    Logger::getInstance().info("STATE: Entrando em 'Verificacao de Ambiente'");
    m_startTime = std::chrono::steady_clock::now();
}

void EnvironmentCheckState::onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) {
    auto now = std::chrono::steady_clock::now();
    double elapsedTime = std::chrono::duration<double>(now - m_startTime).count();

    if (elapsedTime > 10) { // 10 second timeout for the check
        m_controller.transitionTo(std::make_unique<SearchingFaceState>(m_controller));
        return;
    }

    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

    int roiSize = Config::TAMANHO_QUADRO_AMBIENTE;
    cv::Rect roi(
        (grayFrame.cols - roiSize) / 2,
        (grayFrame.rows - roiSize) / 2,
        roiSize,
        roiSize
    );

    roi = roi & cv::Rect(0, 0, grayFrame.cols, grayFrame.rows);

    cv::Mat roiImage = grayFrame(roi);
    double meanBrightness = cv::mean(roiImage)[0];

    cv::rectangle(frame, roi, Config::COLOR_RED, 2);

    std::string brightnessStatus;
    if (meanBrightness >= Config::BRILHO_MINIMO_ADEQUADO) {
        brightnessStatus = "Luminosidade OK (brilho: " + std::to_string(static_cast<int>(meanBrightness)) + ")";
    } else {
        brightnessStatus = "Luminosidade BAIXA (brilho: " + std::to_string(static_cast<int>(meanBrightness)) + ") - Min: " + std::to_string(Config::BRILHO_MINIMO_ADEQUADO);
    }
    
    uiManager.drawCenteredText(frame, "Verificando Ambiente...\n" + brightnessStatus);
}

void EnvironmentCheckState::onExit() {
    Logger::getInstance().info("STATE: Saindo de 'Verificacao de Ambiente'");
}