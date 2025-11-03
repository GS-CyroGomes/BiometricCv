@echo off
setlocal

:: --- Configuração ---
set BUILD_DIR=build
set CMAKE_GENERATOR="MinGW Makefiles"

:: --- ATENÇÃO: Configure o caminho para o OpenCV ---
:: Defina a variável OPENCV_MINGW_DIR com o caminho para a sua instalação do OpenCV
:: compilada com MinGW. Este diretório deve conter os arquivos OpenCVConfig.cmake.
::
:: Exemplo:
:: set OPENCV_MINGW_DIR=C:/path/to/your/opencv-mingw/build
::
set OPENCV_MINGW_DIR=C:/Users/Giusoft/Documents/GitHub/opencv-4.8.0/build/install

:: --- Limpa o build anterior (opcional) ---
if exist %BUILD_DIR% (
    echo Limpando diretório de build anterior...
    rd /s /q %BUILD_DIR%
)
mkdir %BUILD_DIR%
cd %BUILD_DIR%

:: --- Configura o CMake ---
echo Configurando CMake...
cmake -G %CMAKE_GENERATOR% -DOpenCV_DIR="%OPENCV_MINGW_DIR%" ..
if %errorlevel% neq 0 (
    echo ERRO: Falha na configuração do CMake.
    cd ..
    exit /b %errorlevel%
)

:: --- Compila o Projeto ---
echo Compilando projeto...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo ERRO: Falha na compilação.
    cd ..
    exit /b %errorlevel%
)

echo Build concluído com sucesso!
cd ..
endlocal