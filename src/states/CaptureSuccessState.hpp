#ifndef CAPTURE_SUCCESS_STATE_HPP
#define CAPTURE_SUCCESS_STATE_HPP

#include "IAppState.hpp"
#include <chrono>
#include <opencv2/opencv.hpp>

class CaptureSuccessState : public IAppState {
public:
    CaptureSuccessState(StateController& controller, const cv::Mat& capturedFrame, const cv::Rect& faceRect);

    void onEnter() override;
    void onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) override;
    void onExit() override;
    std::string getMessage() const override { return m_message; }

private:
    cv::Mat m_capturedFrame;
    cv::Rect m_faceRect;
    std::chrono::steady_clock::time_point m_displayStartTime;
    std::string m_message;
};

#endif // CAPTURE_SUCCESS_STATE_HPP