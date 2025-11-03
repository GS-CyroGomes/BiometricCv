#ifndef AUDIO_RECORDER_HPP
#define AUDIO_RECORDER_HPP

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "portaudio.h"
#include "sndfile.h"

#include "Config.hpp"
#include "Logger.hpp"

/**
 * @class AudioRecorder
 * @brief Gerencia a gravação de áudio em um arquivo WAV usando PortAudio e LibSndFile.
 */
class AudioRecorder {
public:
    AudioRecorder();
    ~AudioRecorder();

    bool startRecording(const std::string& filename, int sampleRate = 44100, int channels = 1);
    void stopRecording();
    bool isRecording() const { return m_isRecording.load(); }

private:
    std::atomic<bool> m_isRecording;
    std::thread m_recordingThread;
    std::string m_filename;
    int m_sampleRate;
    int m_channels;
    std::mutex m_mutex;

    PaStream* m_audioStream;
    SNDFILE* m_sndFile;
    SF_INFO m_sfinfo;

    std::vector<float> m_buffer;

    static int pa_callback(const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData);

    void recordingLoop();
};

#endif // AUDIO_RECORDER_HPP
