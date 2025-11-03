#ifndef VALIDATING_STATE_HPP
#define VALIDATING_STATE_HPP

#include "IAppState.hpp"

class ValidatingState : public IAppState {
public:
    ValidatingState(StateController& controller);

    void onEnter() override;
    void onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) override;
    void onExit() override;
    std::string getMessage() const override { return m_message; }

private:
    std::string m_message;
};

#endif // VALIDATING_STATE_HPP