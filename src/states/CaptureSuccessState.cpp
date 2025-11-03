#include "CaptureSuccessState.hpp"
#include "SearchingFaceState.hpp"
#include "../StateController.hpp"
#include "../Logger.hpp"
#include "../Config.hpp"

CaptureSuccessState::CaptureSuccessState(StateController& controller, const cv::Mat& capturedFrame, const cv::Rect& faceRect)
    : IAppState(controller), m_capturedFrame(capturedFrame.clone()), m_faceRect(faceRect), m_message("Captura Realizada com Sucesso!") {}

void CaptureSuccessState::onEnter() {
    Logger::getInstance().info("STATE: Entrando em 'Captura Realizada com Sucesso'");
    m_displayStartTime = std::chrono::steady_clock::now();

    // Lógica de salvamento da imagem
    std::string outputPath = Config::OUTPUT_FOLDER + Config::FILENAME_COLLECT;
    try {
        // Recorta a imagem para ter uma proporção mais próxima do rosto
        int scaled_width = m_faceRect.width * Config::CROP_SCALE_FACTOR;
        int scaled_height = m_faceRect.height * Config::CROP_SCALE_FACTOR;
        int x = m_faceRect.x - (scaled_width - m_faceRect.width) / 2;
        int y = m_faceRect.y - (scaled_height - m_faceRect.height) / 2;

        cv::Rect crop_roi(x, y, scaled_width, scaled_height);

        // Garante que o ROI não saia dos limites da imagem
        crop_roi &= cv::Rect(0, 0, m_capturedFrame.cols, m_capturedFrame.rows);

        cv::Mat cropped_image = m_capturedFrame(crop_roi);

        bool success = cv::imwrite(outputPath, cropped_image, {cv::IMWRITE_JPEG_QUALITY, Config::IMAGE_JPEG_QUALITY});

        if (success) {
            Logger::getInstance().info("Imagem biométrica salva em: " + outputPath);
        } else {
            Logger::getInstance().error("Falha ao salvar imagem biométrica em: " + outputPath);
            m_message = "Falha ao salvar imagem";
        }
    } catch (const cv::Exception& ex) {
        Logger::getInstance().error("Erro do OpenCV ao salvar a imagem: " + std::string(ex.what()));
        m_message = "Erro ao processar imagem";
    }
}

void CaptureSuccessState::onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) {
    uiManager.drawCenteredText(frame, m_message);

    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - m_displayStartTime).count() > Config::POST_CAPTURE_DISPLAY_SECONDS) {
        m_controller.transitionTo(std::make_unique<SearchingFaceState>(m_controller)); // Volta para buscar novo rosto
    }
}

void CaptureSuccessState::onExit() {
    Logger::getInstance().info("STATE: Saindo de 'Captura Realizada com Sucesso'");
}