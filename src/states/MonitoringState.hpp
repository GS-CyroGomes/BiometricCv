#ifndef MONITORING_STATE_HPP
#define MONITORING_STATE_HPP

#include "IAppState.hpp"
#include <chrono>

class MonitoringState : public IAppState {
public:
    MonitoringState(StateController& controller);

    void onEnter() override;
    void onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) override;
    void onExit() override;

private:
    std::chrono::steady_clock::time_point m_lastFrameTime;
    double m_currentFPS = 0.0;
    double m_fpsMax = 0.0;
    double m_fpsMin = 9999.0;
};

#endif // MONITORING_STATE_HPP