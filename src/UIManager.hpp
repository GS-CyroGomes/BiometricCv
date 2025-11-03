#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include "Config.hpp"

/**
 * @class UIManager
 * @brief Gerencia toda a saída visual: janelas, frames desenhos de feedeback.
 */
class UIManager {
public:
    UIManager(const std::string& windowName);
    ~UIManager();

    //Cria e configura a janela de exibição
    void createWindow(int width, int height);
    
    //Exibe o frame na janela
    void showFrame(const cv::Mat& frame);
    
    //Desenha texto centralizado no frame
    void drawCenteredText(cv::Mat& frame, const std::string& text);

    // Desenha uma elipse (será usada para guiar o posicionamento do rosto)
    void drawEllipse(cv::Mat& frame, const cv::Point& center, const cv::Size& axes, const cv::Scalar& color);

    // Espera por uma tecla por um tempo especificado
    int waitKey(int delay);

private:
    std::string m_windowName;
    bool m_isWindowCreated = false;
};
#endif