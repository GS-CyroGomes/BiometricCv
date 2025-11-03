#ifndef CAPTURE_FAILED_STATE_HPP
#define CAPTURE_FAILED_STATE_HPP

#include "IAppState.hpp"
#include <chrono>

class CaptureFailedState : public IAppState {
public:
    CaptureFailedState(StateController& controller, const std::string& errorMessage);

    void onEnter() override;
    void onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) override;
    void onExit() override;
    std::string getMessage() const override { return m_message; }

private:
    std::string m_errorMessage;
    std::chrono::steady_clock::time_point m_displayStartTime;
    std::string m_message;
};

#endif // CAPTURE_FAILED_STATE_HPP