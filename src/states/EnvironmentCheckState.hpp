#ifndef ENVIRONMENT_CHECK_STATE_HPP
#define ENVIRONMENT_CHECK_STATE_HPP

#include "IAppState.hpp"
#include <chrono>

class EnvironmentCheckState : public IAppState {
public:
    EnvironmentCheckState(StateController& controller);

    void onEnter() override;
    void onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) override;
    void onExit() override;

private:
    std::chrono::steady_clock::time_point m_startTime;
};

#endif // ENVIRONMENT_CHECK_STATE_HPP