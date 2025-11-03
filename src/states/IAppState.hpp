#ifndef I_APP_STATE_HPP
#define I_APP_STATE_HPP

#include "../FaceProcessor.hpp"
#include "../UIManager.hpp"

// Forward declaration para evitar dependência circular
class StateController;

class IAppState {
public:
    virtual ~IAppState() = default;

    // Chamado quando o estado se torna ativo
    virtual void onEnter() = 0;

    // Chamado a cada frame do loop principal
    virtual void onUpdate(const FaceAnalysisResult& analysisResult, cv::Mat& frame, UIManager& uiManager) = 0;

    // Chamado quando o estado deixa de ser ativo
    virtual void onExit() = 0;

    virtual std::string getMessage() const = 0;

protected:
    // Construtor protegido para que a interface não possa ser instanciada diretamente
    IAppState(StateController& controller) : m_controller(controller) {}

    StateController& m_controller;
};

#endif // I_APP_STATE_HPP