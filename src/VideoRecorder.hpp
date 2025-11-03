#ifndef VIDEO_RECORDER_HPP
#define VIDEO_RECORDER_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <mutex>
#include <chrono>

#include "Config.hpp"
#include "Logger.hpp"

/**
 * @class VideoRecorder
 * @brief Gerencia a gravação de frames de vídeo em um arquivo.
 */
class VideoRecorder {
public:
    VideoRecorder();
    ~VideoRecorder();

    bool startRecording(const std::string& filename, int frameWidth, int frameHeight, double fps);
    void stopRecording();
    void writeFrame(const cv::Mat& frame);
    bool isRecording() const;
    double getRecordedDuration() const;

private:
    cv::VideoWriter m_videoWriter;
    std::mutex m_mutex;
    bool m_isRecording = false;
    std::chrono::high_resolution_clock::time_point m_startTime;
};

#endif // VIDEO_RECORDER_HPP
