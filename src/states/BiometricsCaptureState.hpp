#ifndef BIOMETRICS_CAPTURE_STATE_HPP
#define BIOMETRICS_CAPTURE_STATE_HPP

#include "IAppState.hpp"
#include <chrono>

class BiometricsCaptureState : public IAppState {
public:
    BiometricsCaptureState(StateController& controller);

    void onEnter() override;
    void onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) override;
    void onExit() override;
    std::string getMessage() const override { return m_message; }

private:
    enum CapturePhase {
        GUIDING,
        COUNTDOWN,
        CAPTURING,
        POST_CAPTURE
    };

    CapturePhase m_currentPhase = GUIDING;
    std::chrono::steady_clock::time_point m_phaseStartTime;
    int m_countdownValue = 0;
    bool m_captureTriggered = false;
    std::string m_message;

    void saveImage(const cv::Mat& frame, const cv::Rect& faceRect);
};

#endif // BIOMETRICS_CAPTURE_STATE_HPP