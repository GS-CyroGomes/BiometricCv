#include "UIManager.hpp"
#include "Config.hpp"
#include <iostream>

UIManager::UIManager(const std::string& windowName) : m_windowName(windowName) {

}

UIManager::~UIManager() {
    if (m_isWindowCreated) {
        cv::destroyWindow(m_windowName);
    }
}

void UIManager::createWindow(int width, int height) {
    if (!m_isWindowCreated) {
        cv::namedWindow(m_windowName, cv::WINDOW_NORMAL);
        cv::resizeWindow(m_windowName, width, height);
        m_isWindowCreated = true;
    }
}

void UIManager::showFrame(const cv::Mat& frame) {
    if (m_isWindowCreated) {
        cv::imshow(m_windowName, frame);
    }
}

void UIManager::drawCenteredText(cv::Mat& frame, const std::string& text) {
    if (frame.empty()) return ;

    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.0;
    int thickness = 2;
    int baseline = 0;

    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
    cv::Point textOrg((frame.cols - textSize.width) / 2, frame.rows - 40);

    cv::rectangle(frame, textOrg + cv::Point(0, baseline), 
                  textOrg + cv::Point(textSize.width, -textSize.height), 
                  Config::COLOR_BLACK, cv::FILLED);
    cv::putText(frame, text, textOrg, fontFace, fontScale, Config::COLOR_WHITE, thickness);
}

void UIManager::drawEllipse(cv::Mat& frame, const cv::Point& center, const cv::Size& axes, const cv::Scalar& color) {
    if (frame.empty()) return ;
    cv::ellipse(frame, center, axes, 0, 0, 360, color, 2);
}

int UIManager::waitKey(int delay) {
    return cv::waitKey(delay);
}