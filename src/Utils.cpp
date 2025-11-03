#define _USE_MATH_DEFINES
#include "Utils.hpp"

#include <windows.h> // Para GetSystemMetrics e GetModuleFileNameA
#include <shlwapi.h> // Para PathFileExists e PathRemoveFileSpecA
#include <iostream> // Para debug

#pragma comment(lib, "Shlwapi.lib") // Link com Shlwapi.lib para PathFileExists

namespace Utils {

    std::string getExecutableDir() {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        PathRemoveFileSpecA(buffer);
        return std::string(buffer);
    }

    void getScreenResolution(int& horizontal, int& vertical) {
        horizontal = GetSystemMetrics(SM_CXSCREEN);
        vertical = GetSystemMetrics(SM_CYSCREEN);
    }

    bool fileExists(const std::string& nome) {
        return PathFileExistsA(nome.c_str());
    }

    double calcAngle(double x1, double y1, double x2, double y2) {
        return std::atan2(y2 - y1, x2 - x1) * 180.0 / M_PI;
    }

    double calcDistance(double x1, double y1, double x2, double y2) {
        return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    }

    double calcPercRatio(double valor, double total) {
        if (total == 0) return 0;
        return (valor * 100.0) / total;
    }

    std::string imageToBase64(const cv::Mat& image) {
        if (image.empty()) {
            return "";
        }
        std::vector<uchar> buf;
        cv::imencode(".jpg", image, buf);
        std::string str(buf.begin(), buf.end());
        return base64_encode(str);
    }

} // namespace Utils
