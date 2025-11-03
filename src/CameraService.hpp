#ifndef CAMERA_SERVICE_HPP
#define CAMERA_SERVICE_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include "Config.hpp"

/**
 * @file CameraService.hpp
 * @brief Declara a classe CameraService para gerenciar a captura de imagens da camera.
 * 
 * Responsavel por inicializar a camera, configurar parametros de captura,
 * capturar frames e liberar recursos associados a camera.
 */
class CameraService {
public:
    CameraService(int cameraIndex = 0);
    ~CameraService();

    bool open();
    void release();
    bool readFrame(cv::Mat& frame);
    bool isOpened() const;

    int getWidth() const { return m_width; };
    int getHeight() const { return m_height; };

private:
    void configureResolution();

    cv::VideoCapture m_capture;
    int m_cameraIndex;
    int m_width;
    int m_height;
};
#endif // CAMERA_SERVICE_HPP