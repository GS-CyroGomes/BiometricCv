#include <iostream>
#include <stdexcept>
#include <functional>
#include <mutex>
#include <filesystem>
#include <chrono>
#include <iomanip>

#include "Config.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include "CameraService.hpp"
#include "UIManager.hpp"
#include "FaceProcessor.hpp"
#include "StateController.hpp"
#include "SocketServer.hpp"
#include "StreamServer.hpp"
#include "FeedbackServer.hpp"

namespace fs = std::filesystem;

class Application {
public:
    Application(int argc, char** argv)
        : m_basePath(Utils::getExecutableDir()),
          m_cameraService(0),
          m_uiManager("Captura Biometrica"),
          m_faceProcessor(m_basePath),
          m_socketServer(Config::SOCKET_PORT, [this](const std::string& cmd) { this->processExternalCommand(cmd); }),
          m_streamServer(Config::STREAM_PORT),
          m_feedbackServer(Config::FEEDBACK_PORT) {
        Logger::getInstance().init(m_basePath + "/gCV.log");
        Logger::getInstance().info("Application constructor called.");
        Logger::getInstance().info("Diretorio base da aplicacao: " + m_basePath);
        
        // Create captures directory if it doesn't exist
        fs::create_directory(m_basePath + "/" + Config::OUTPUT_FOLDER);
    }

    void run() {
        Logger::getInstance().info("Iniciando Biometric Face Capture v1.0.0");

        m_streamServer.start();
        m_feedbackServer.start();
        m_socketServer.start();

        if (!m_cameraService.open()) {
            Logger::getInstance().error("Falha critica: Nao foi possivel iniciar o CameraService.");
            m_isRunning = false;
        }

        if (m_isRunning) {
            Logger::getInstance().info("Resolucao da camera configurada para " + std::to_string(m_cameraService.getWidth()) + "x" + std::to_string(m_cameraService.getHeight()));
            m_faceProcessor.initialize(m_cameraService.getWidth(), m_cameraService.getHeight());
            // m_uiManager.createWindow(m_cameraService.getWidth(), m_cameraService.getHeight());
        }

        cv::Mat currentFrame;
        while (m_isRunning) {
            if (!m_cameraService.readFrame(currentFrame) || currentFrame.empty()) {
                Logger::getInstance().warning("Falha ao ler frame da camera. Encerrando...");
                m_isRunning = false;
                continue;
            }
            
            {
                std::lock_guard<std::mutex> lock(m_frameMutex);
                m_latestFrame = currentFrame.clone();
            }

            FaceAnalysisResult analysisResult = m_faceProcessor.processFrame(currentFrame);

            m_stateController.update(analysisResult, currentFrame, m_uiManager);

            // m_uiManager.showFrame(currentFrame);
            m_streamServer.streamFrame(currentFrame);
            m_feedbackServer.sendFeedback(m_stateController.getCurrentStateMessage());

            // int key = m_uiManager.waitKey(1);
            // if (key == 27) { // ESC
            //     m_isRunning = false;
            // }
        }
    }

    ~Application() {
        Logger::getInstance().info("Finalizando a aplicacao...");
        m_socketServer.stop();
        m_streamServer.stop();
        m_feedbackServer.stop();
    }

private:
    void processExternalCommand(const std::string& command) {
        Logger::getInstance().info("Comando recebido: " + command);
        if (command == "START_CAPTURE") {
            m_stateController.requestCapture();
        } else if (command == "EXIT") {
            m_isRunning = false;
        } else if (command == "SAVE_PHOTO") {
            std::lock_guard<std::mutex> lock(m_frameMutex);
            if (!m_latestFrame.empty()) {
                auto now = std::chrono::system_clock::now();
                auto in_time_t = std::chrono::system_clock::to_time_t(now);
                std::stringstream ss;
                ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
                std::string filename = m_basePath + "/" + Config::OUTPUT_FOLDER + "capture_" + ss.str() + ".jpg";
                
                std::vector<int> params;
                params.push_back(cv::IMWRITE_JPEG_QUALITY);
                params.push_back(Config::IMAGE_JPEG_QUALITY);

                if(cv::imwrite(filename, m_latestFrame, params)) {
                    Logger::getInstance().info("Imagem salva em: " + filename);
                } else {
                    Logger::getInstance().error("Falha ao salvar imagem em: " + filename);
                }
            }
        }
    }

    std::string m_basePath;
    bool m_isRunning = true;
    cv::Mat m_latestFrame;
    std::mutex m_frameMutex;
    CameraService m_cameraService;
    UIManager m_uiManager;
    FaceProcessor m_faceProcessor;
    StateController m_stateController;
    SocketServer m_socketServer;
    StreamServer m_streamServer;
    FeedbackServer m_feedbackServer;
};

int main(int argc, char** argv) {
    try {
        Application app(argc, argv);
        app.run();
    } catch (const std::exception& e) {
        Logger::getInstance().error("Erro critico na aplicacao: " + std::string(e.what()));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
