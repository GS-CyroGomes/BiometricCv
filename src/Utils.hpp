#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <stdexcept>

#include "base64.h"

/**
 * @namespace Utils
 * @brief Namespace para funções de utilidade geral.
 */
namespace Utils {

    // Funções de sistema/arquivo
    std::string getExecutableDir();
    void getScreenResolution(int& horizontal, int& vertical);
    bool fileExists(const std::string& nome);

    // Funções matemáticas/geometria
    double calcAngle(double x1, double y1, double x2, double y2);
    double calcDistance(double x1, double y1, double x2, double y2);
    double calcPercRatio(double valor, double total);

    // Funções de codificação/decodificação
    std::string imageToBase64(const cv::Mat& image);

} // namespace Utils

#endif // UTILS_HPP
