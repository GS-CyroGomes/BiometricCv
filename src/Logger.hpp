#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <iostream>

/**
 * @class Logger
 * @brief Sistema de logging simples com suporte a diferentes níveis e saída para arquivo/console.
 * Implementado como um Singleton para acesso global.
 */
class Logger {
public:
    enum class LogLevel {
        L_INFO,
        L_WARNING,
        L_ERROR
    };

    // Retorna a única instância do Logger
    static Logger& getInstance();

    // Desabilita cópia e atribuição
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Inicializa o logger (abre o arquivo de log)
    bool init(const std::string& filename, bool logToFile = true);
    void shutdown();

    // Métodos de logging
    void log(LogLevel level, const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);

private:
    Logger(); // Construtor privado para Singleton
    ~Logger(); // Destrutor privado

    std::ofstream m_logFile;
    std::mutex m_mutex;
    bool m_logToFile = false;

    std::string getTimestamp();
    std::string logLevelToString(LogLevel level);
};

#endif // LOGGER_HPP
