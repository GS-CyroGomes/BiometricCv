#ifndef STATE_CONTROLLER_HPP
#define STATE_CONTROLLER_HPP

#include <memory>
#include "states/IAppState.hpp"

class StateController {
public:
    StateController();

    void transitionTo(std::unique_ptr<IAppState> newState);
    void update(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager);
    std::string getCurrentStateMessage() const;

    // Novo método para solicitar captura via comando externo
    void requestCapture();
    void requestBiometricsCapture();
    bool isCaptureRequested() const { return m_captureRequested; }
    bool isBiometrics() const { return m_isBiometrics; }
    void resetCaptureRequest() { m_captureRequested = false; }

private:
    std::unique_ptr<IAppState> m_currentState;
    bool m_captureRequested = false; // Flag para sinalizar pedido de captura
    bool m_isBiometrics = false;
};

#endif // STATE_CONTROLLER_HPP