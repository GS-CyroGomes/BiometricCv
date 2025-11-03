cmake_minimum_required(VERSION 3.10)
project(BiometricCapture CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona a definição NOMINMAX para evitar conflitos com os cabeçalhos do Windows
add_compile_definitions(NOMINMAX)

# --- Configuração manual do OpenCV ---
# Aponta para a instalação local do OpenCV dentro do projeto
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# --- Adiciona os arquivos fonte ---
add_executable(biometric_capture
    src/main.cpp
    src/CameraService.cpp
    src/UIManager.cpp
    src/FaceProcessor.cpp
    src/StateController.cpp
    src/states/SearchingFaceState.cpp
    src/states/ValidatingState.cpp
    src/states/BiometricsCaptureState.cpp
    src/states/CaptureSuccessState.cpp
    src/states/CaptureFailedState.cpp
    src/SocketServer.cpp
    src/SocketServer.cpp
    src/states/EnvironmentCheckState.cpp
    src/states/MonitoringState.cpp
    src/AudioRecorder.cpp
    src/VideoRecorder.cpp
    src/Logger.cpp
    src/Utils.cpp
    src/StreamServer.cpp
    src/FeedbackServer.cpp
    src/base64.cpp
)

# --- Linka as bibliotecas ---
# Linka diretamente com a DLL "world" do OpenCV que contém todos os módulos
target_link_libraries(biometric_capture "${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/lib/libopencv_world480.dll")

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs")
target_link_libraries(biometric_capture portaudio sndfile)

# --- Bibliotecas específicas da plataforma ---
if(WIN32)
    target_link_libraries(biometric_capture ws2_32 mswsock shlwapi)
endif()

# --- Comandos Pós-Build ---

# Cria o diretório 'proc'
add_custom_command(TARGET biometric_capture POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:biometric_capture>/proc
    COMMENT "Criando diretório proc"
)

# Copia a pasta 'models' para o diretório de build
add_custom_command(TARGET biometric_capture POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/models
            $<TARGET_FILE_DIR:biometric_capture>/models
    COMMENT "Copiando diretório models"
)
