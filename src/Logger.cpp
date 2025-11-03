#include "Logger.hpp"
#include "Config.hpp"

// Inicialização estática da instância única
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    // Construtor privado
}

Logger::~Logger() {
    shutdown();
}

bool Logger::init(const std::string& filename, bool logToFile) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logToFile = logToFile;
    if (m_logToFile) {
        m_logFile.open(filename, std::ios::app); // Abre em modo append
        if (!m_logFile.is_open()) {
            std::cerr << "!! Erro ao abrir arquivo de log: " << filename << std::endl;
            m_logToFile = false; // Desabilita log para arquivo se falhar
            return false;
        }
        m_logFile << getTimestamp() << " [INFO] Logger inicializado." << std::endl;
    }
    std::cout << getTimestamp() << " [INFO] Logger inicializado." << std::endl;
    return true;
}

void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_logFile.is_open()) {
        m_logFile << getTimestamp() << " [INFO] Logger encerrado." << std::endl;
        m_logFile.close();
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string formattedMessage = getTimestamp() + " " + logLevelToString(level) + " " + message;

    // Saída para console
    std::cout << formattedMessage << std::endl;

    // Saída para arquivo
    if (m_logToFile && m_logFile.is_open()) {
        m_logFile << formattedMessage << std::endl;
    }
}

void Logger::info(const std::string& message) {
    log(LogLevel::L_INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::L_WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::L_ERROR, message);
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "[%Y-%m-%d %H:%M:%S]");
    return ss.str();
}

std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::L_INFO: return "[INFO]";
        case LogLevel::L_WARNING: return "[WARNING]";
        case LogLevel::L_ERROR: return "[ERROR]";
        default: return "[UNKNOWN]";
    }
}
