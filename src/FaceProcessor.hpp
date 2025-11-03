#ifndef FACE_PROCESSOR_HPP
#define FACE_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <string>
#include <vector>
#include "Config.hpp"

// Estrutura para retornar o resultado completo da análise facial
struct FaceAnalysisResult {
    bool faceFound = false;
    bool isCompliant = false;
    cv::Rect faceRect;
    std::vector<std::vector<cv::Point2f>> landmarks;
    std::string failureReason;

    // Detalhamento das validações
    bool isPositionValid = false;
    bool isCentralizationValid = false;
    bool isBlurValid = false;
    bool isBackgroundValid = false;
    bool isLightingValid = false;
    bool hasGlasses = true; // Inicia como true para falhar por padrão
    bool hasReflections = true;
    bool areEyesOpen = false;
    bool isMouthClosed = false;
    bool isHeadPoseValid = false;
};

class FaceProcessor {
public:
    FaceProcessor(const std::string& basePath);
    void initialize(int frameWidth, int frameHeight);

    // Processa um frame e retorna o resultado da análise
    FaceAnalysisResult processFrame(const cv::Mat& frame);

private:
    // Métodos de validação baseados nos requisitos
    bool validateFacePosition(const cv::Rect& faceRect, int frameWidth, int frameHeight, std::string& reason);
    bool validateEyeStatus(const std::vector<cv::Point2f>& landmarks, std::string& reason);
    bool validateMouthStatus(const std::vector<cv::Point2f>& landmarks, std::string& reason);
    bool validateHeadPose(const std::vector<cv::Point2f>& landmarks, std::string& reason);

    float calculateEAR(const std::vector<cv::Point2f>& eyePoints);

    std::string m_basePath;
    cv::Ptr<cv::FaceDetectorYN> m_faceDetector;
    cv::Ptr<cv::face::Facemark> m_landmarkDetector;
};

#endif // FACE_PROCESSOR_HPP