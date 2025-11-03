#include "FaceProcessor.hpp"
#include "Logger.hpp"
#include "Config.hpp"
#include <iostream>

#define NOMINMAX

FaceProcessor::FaceProcessor(const std::string& basePath)
    : m_basePath(basePath) {
}

void FaceProcessor::initialize(int frameWidth, int frameHeight) {
    std::string detectionModelPath = m_basePath + "/models/face_detection_yunet_2023mar.onnx";
    std::string landmarkModelPath = m_basePath + "/models/lbfmodel.yaml";

    m_faceDetector = cv::FaceDetectorYN::create(detectionModelPath, "", cv::Size(frameWidth, frameHeight), 0.9f, 0.3f, 5000);
    
    m_landmarkDetector = cv::face::FacemarkLBF::create();
    m_landmarkDetector->loadModel(landmarkModelPath);

    Logger::getInstance().info("Modelos de deteccao facial e landmarks carregados de: " + m_basePath);
}

FaceAnalysisResult FaceProcessor::processFrame(const cv::Mat& frame) {
    FaceAnalysisResult result;
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

    // 1. Detecção de Face
    cv::Mat faces;
    if (m_faceDetector.empty()) {
        result.failureReason = "Detector facial nao inicializado";
        return result;
    }
    m_faceDetector->detect(frame, faces);

    if (faces.rows < 1) {
        result.failureReason = "Nenhum rosto detectado";
        return result;
    }
    if (faces.rows > 1) {
        result.failureReason = "Multiplos rostos detectados";
        return result;
    }

    result.faceFound = true;
    result.faceRect = cv::Rect(faces.at<float>(0, 0), faces.at<float>(0, 1),
                               faces.at<float>(0, 2), faces.at<float>(0, 3));

    // 2. Detecção de Pontos Faciais
    std::vector<cv::Rect> faceRects = {result.faceRect};
    std::vector<std::vector<cv::Point2f>> landmarks;
    if (!m_landmarkDetector->fit(frame, faceRects, landmarks)) {
        result.faceFound = false; 
        result.failureReason = "Nao foi possivel detectar os pontos faciais";
        return result;
    }
    result.landmarks = landmarks;

    // 3. Validações de Qualidade
    std::string reason;
    if (!validateFacePosition(result.faceRect, frame.cols, frame.rows, reason) ||
        !validateEyeStatus(result.landmarks[0], reason) ||
        !validateMouthStatus(result.landmarks[0], reason) ||
        !validateHeadPose(result.landmarks[0], reason)) {
        result.isCompliant = false;
        result.failureReason = reason;
    } else {
        result.isCompliant = true;
        result.failureReason = "Pronto para captura";
    }

    return result;
}

bool FaceProcessor::validateFacePosition(const cv::Rect& faceRect, int frameWidth, int frameHeight, std::string& reason) {
    float faceWidthPercent = (float)faceRect.width / frameWidth;
    float faceHeightPercent = (float)faceRect.height / frameHeight;

    if (faceWidthPercent < Config::MIN_FACE_WIDTH_PERCENT) { reason = "Aproxime-se da camera"; return false; }
    if (faceWidthPercent > Config::MAX_FACE_WIDTH_PERCENT) { reason = "Afaste-se da camera"; return false; }
    if (faceHeightPercent < Config::MIN_FACE_HEIGHT_PERCENT) { reason = "Centralize o rosto verticalmente"; return false; }
    if (faceHeightPercent > Config::MAX_FACE_HEIGHT_PERCENT) { reason = "Centralize o rosto verticalmente"; return false; }
    
    return true;
}

float FaceProcessor::calculateEAR(const std::vector<cv::Point2f>& eyePoints) {
    float a = cv::norm(eyePoints[1] - eyePoints[5]);
    float b = cv::norm(eyePoints[2] - eyePoints[4]);
    float c = cv::norm(eyePoints[0] - eyePoints[3]);
    return (a + b) / (2.0f * c);
}

bool FaceProcessor::validateEyeStatus(const std::vector<cv::Point2f>& landmarks, std::string& reason) {
    const float EYE_AR_THRESH = 0.22f;

    std::vector<cv::Point2f> leftEye(landmarks.begin() + 36, landmarks.begin() + 42);
    std::vector<cv::Point2f> rightEye(landmarks.begin() + 42, landmarks.begin() + 48);

    float leftEAR = calculateEAR(leftEye);
    float rightEAR = calculateEAR(rightEye);

    if (leftEAR < EYE_AR_THRESH || rightEAR < EYE_AR_THRESH) {
        reason = "Mantenha os olhos abertos";
        return false;
    }
    return true;
}

bool FaceProcessor::validateMouthStatus(const std::vector<cv::Point2f>& landmarks, std::string& reason) {
    const float MOUTH_AR_THRESH = 0.4f;
    float a = cv::norm(landmarks[62] - landmarks[66]);
    float b = cv::norm(landmarks[60] - landmarks[64]);

    if ((a / b) > MOUTH_AR_THRESH) {
        reason = "Mantenha a boca fechada";
        return false;
    }
    return true;
}

bool FaceProcessor::validateHeadPose(const std::vector<cv::Point2f>& landmarks, std::string& reason) {
    float eyeDistHorizontal = cv::norm(landmarks[36] - landmarks[45]);
    float noseTipToChinY = std::abs(landmarks[30].y - landmarks[8].y);

    if (eyeDistHorizontal / noseTipToChinY < 0.6) { 
        reason = "Mantenha o rosto reto e de frente para a camera";
        return false;
    }
    return true;
}