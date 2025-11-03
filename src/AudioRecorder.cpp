#include "AudioRecorder.hpp"

#define FRAMES_PER_BUFFER 512
#define PA_SAMPLE_TYPE paFloat32

AudioRecorder::AudioRecorder() : m_isRecording(false), m_audioStream(nullptr), m_sndFile(nullptr) {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        Logger::getInstance().error("Erro ao inicializar PortAudio: " + std::string(Pa_GetErrorText(err)));
    }
}

AudioRecorder::~AudioRecorder() {
    stopRecording();
    PaError err = Pa_Terminate();
    if (err != paNoError) {
        Logger::getInstance().error("Erro ao finalizar PortAudio: " + std::string(Pa_GetErrorText(err)));
    }
}

int AudioRecorder::pa_callback(const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData) {
    AudioRecorder* recorder = static_cast<AudioRecorder*>(userData);
    const float *in = (const float*)inputBuffer;

    if (inputBuffer == nullptr) {
        return paContinue;
    }

    std::lock_guard<std::mutex> lock(recorder->m_mutex);
    if (recorder->m_isRecording) {
        recorder->m_buffer.insert(recorder->m_buffer.end(), in, in + framesPerBuffer * recorder->m_channels);
    }
    
    return paContinue;
}

bool AudioRecorder::startRecording(const std::string& filename, int sampleRate, int channels) {
    if (m_isRecording.load()) {
        Logger::getInstance().warning("Gravacao de audio ja esta ativa.");
        return false;
    }

    m_filename = filename;
    m_sampleRate = sampleRate;
    m_channels = channels;
    m_buffer.clear();

    m_sfinfo.samplerate = m_sampleRate;
    m_sfinfo.channels = m_channels;
    m_sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    m_sndFile = sf_open(m_filename.c_str(), SFM_WRITE, &m_sfinfo);
    if (!m_sndFile) {
        Logger::getInstance().error("Erro ao abrir arquivo de audio para escrita: " + m_filename + " - " + std::string(sf_strerror(nullptr)));
        return false;
    }
    
    PaStreamParameters inputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        Logger::getInstance().error("Nenhum dispositivo de entrada de audio encontrado.");
        sf_close(m_sndFile);
        m_sndFile = nullptr;
        return false;
    }
    inputParameters.channelCount = m_channels;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    PaError err = Pa_OpenStream(
        &m_audioStream,
        &inputParameters,
        nullptr, 
        m_sampleRate,
        FRAMES_PER_BUFFER,
        paClipOff, 
        pa_callback,
        this);

    if (err != paNoError) {
        Logger::getInstance().error("Erro ao abrir stream de audio PortAudio: " + std::string(Pa_GetErrorText(err)));
        sf_close(m_sndFile);
        m_sndFile = nullptr;
        return false;
    }

    err = Pa_StartStream(m_audioStream);
    if (err != paNoError) {
        Logger::getInstance().error("Erro ao iniciar stream de audio PortAudio: " + std::string(Pa_GetErrorText(err)));
        Pa_CloseStream(m_audioStream);
        m_audioStream = nullptr;
        sf_close(m_sndFile);
        m_sndFile = nullptr;
        return false;
    }

    m_isRecording.store(true);
    Logger::getInstance().info("Gravacao de audio iniciada: " + m_filename);
    
    m_recordingThread = std::thread([this]() { recordingLoop(); });

    return true;
}

void AudioRecorder::stopRecording() {
    if (m_isRecording.load()) {
        m_isRecording.store(false);

        if (m_recordingThread.joinable()) {
            m_recordingThread.join();
        }

        PaError err = Pa_StopStream(m_audioStream);
        if (err != paNoError) {
            Logger::getInstance().error("Erro ao parar stream de audio PortAudio: " + std::string(Pa_GetErrorText(err)));
        }
        err = Pa_CloseStream(m_audioStream);
        if (err != paNoError) {
            Logger::getInstance().error("Erro ao fechar stream de audio PortAudio: " + std::string(Pa_GetErrorText(err)));
        }
        m_audioStream = nullptr;

        if (m_sndFile) {
            sf_close(m_sndFile);
            m_sndFile = nullptr;
        }

        Logger::getInstance().info("Gravacao de audio parada.");
    }
}

void AudioRecorder::recordingLoop() {
    while (m_isRecording.load()) {
        std::vector<float> tempBuffer;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_buffer.empty()) {
                tempBuffer.swap(m_buffer);
            }
        }

        if (!tempBuffer.empty() && m_sndFile) {
            sf_write_float(m_sndFile, tempBuffer.data(), tempBuffer.size());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_buffer.empty() && m_sndFile) {
        sf_write_float(m_sndFile, m_buffer.data(), m_buffer.size());
        m_buffer.clear();
    }
}
