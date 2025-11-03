#ifndef SEARCHING_FACE_STATE_HPP
#define SEARCHING_FACE_STATE_HPP

#include "IAppState.hpp"

class SearchingFaceState : public IAppState {
public:
    SearchingFaceState(StateController& controller);

    void onEnter() override;
    void onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) override;
    void onExit() override;
    std::string getMessage() const override { return m_message; }

private:
    std::string m_message;
};

#endif // SEARCHING_FACE_STATE_HPP