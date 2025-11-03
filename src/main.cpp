#include <iostream>
#include <stdexcept>
#include <functional>

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
            m_uiManager.createWindow(m_cameraService.getWidth(), m_cameraService.getHeight());
        }

        cv::Mat currentFrame;
        while (m_isRunning) {
            if (!m_cameraService.readFrame(currentFrame) || currentFrame.empty()) {
                Logger::getInstance().warning("Falha ao ler frame da camera. Encerrando...");
                m_isRunning = false;
                continue;
            }

            FaceAnalysisResult analysisResult = m_faceProcessor.processFrame(currentFrame);

            m_stateController.update(analysisResult, currentFrame, m_uiManager);

            m_uiManager.showFrame(currentFrame);
            m_streamServer.streamFrame(currentFrame);
            m_feedbackServer.sendFeedback(m_stateController.getCurrentStateMessage());

            int key = m_uiManager.waitKey(1);
            if (key == 27) { // ESC
                m_isRunning = false;
            }
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
        }
    }

    std::string m_basePath;
    bool m_isRunning = true;
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
