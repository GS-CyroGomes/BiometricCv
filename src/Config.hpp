#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <opencv2/opencv.hpp>
#include <string>

namespace Config {
    // =================================================================
    // Informações da Aplicação
    // =================================================================
    const std::string APP_NAME = "Biometric Face Capture";
    const std::string APP_VERSION = "1.0.0";

    // =================================================================
    // Requisitos ISO/IEC 19794-5
    // =================================================================
    constexpr int MIN_IMAGE_WIDTH = 640;
    constexpr int MIN_IMAGE_HEIGHT = 480;
    constexpr double MIN_FACE_WIDTH_PERCENT = 0.50; // 50% da largura da imagem;
    constexpr double MAX_FACE_WIDTH_PERCENT = 0.75; // 75% da largura da imagem;
    constexpr double MIN_FACE_HEIGHT_PERCENT = 0.60; // 60% da altura da imagem;
    constexpr double MAX_FACE_HEIGHT_PERCENT = 0.90; // 80% da altura
    const std::string IMAGE_FORMAT = ".jpg"; // Formato de imagem aceito
    constexpr int IMAGE_JPEG_QUALITY = 90; // Qualidade JPEG (0-100)
    constexpr double CROP_SCALE_FACTOR = 1.5; // Fator de escala para o corte da imagem final
    
    // =================================================================
    // Parâmetros da Câmera e Captura
    // =================================================================
    constexpr double CAPTURE_FPS = 30.0; // Frames por segundo para captura
    constexpr int MAX_CAMERA_FAILURES = 5; // Número máximo de falhas permitidas na câmera

    // =================================================================
    // Configurações da UI (Cores para Feedback Visual)
    // =================================================================
    const cv::Scalar COLOR_GREEN = cv::Scalar(0, 255, 0);     // Sucesso, posição correta
    const cv::Scalar COLOR_RED = cv::Scalar(0, 0, 255);       // Erro, posição incorreta
    const cv::Scalar COLOR_BLUE = cv::Scalar(255, 0, 0);      // Informação
    const cv::Scalar COLOR_WHITE = cv::Scalar(255, 255, 255); // Textos
    const cv::Scalar COLOR_BLACK = cv::Scalar(0, 0, 0);       // Fundo de textos

    // =================================================================
    // Configurações de Rede
    // =================================================================
    constexpr short SOCKET_PORT = 7007;
    constexpr short STREAM_PORT = 7008;
    constexpr short FEEDBACK_PORT = 7009;    

    // =================================================================
    // Configurações de Captura
    // =================================================================
    constexpr int CAPTURE_COUNTDOWN_SECONDS = 3; // Contagem regressiva antes da foto
    constexpr int POST_CAPTURE_DISPLAY_SECONDS = 2; // Tempo para exibir mensagem de sucesso/falha
    const std::string OUTPUT_FOLDER = "./captures/"; // Pasta para salvar as imagens
    const std::string FILENAME_COLLECT = "biometric_photo.jpg"; // Nome do arquivo da foto biométrica

    // =================================================================
    // Configurações de Rede
    // =================================================================
    constexpr short PORTA_SERVIDOR = 7007; // Porta para comandos
    constexpr short PORTA_STREAMING = 7008; // Porta para streaming de vídeo
    constexpr short PORTA_FEEDBACK = 7009; // Porta para feedback de texto

    // =================================================================
    // @note Para o teste de ambiente
    // =================================================================
    constexpr int TAMANHO_QUADRO_AMBIENTE = 150;
    constexpr int BRILHO_MINIMO_ADEQUADO = 150;

    // =================================================================
    // @note Para a coleta de biometria
    // =================================================================
    constexpr int MAX_FRAMES_COLETA_FACIAL = 45; // padrao 45
    constexpr int MAX_FRAUDE_MULTIPLAS_FACES = 1;
    constexpr int MAX_FRAUDE_IMAGEM_BORRADA = 20;
    constexpr int MAX_FRAUDE_IMAGEM_COM_BARRAS = 10;
    constexpr int MAX_TOLERANCIA_CENTRALIZACAO = 40;
    constexpr int MAX_FRAMES_CENTRALIZACAO_FACIAL = 30; // Padrão 20
    constexpr double DESFOQUE_MAXIMO_PERMITIDO_COLETA = 55.0; // Quanto maior o numero, mais focado está.
    constexpr double DESFOQUE_MAXIMO_PERMITIDO_BIOMETRIA = 46.0; // Adicionado: Biometria exige foto mais próxima.

    // =================================================================
    // @note Para o monitoramento
    // =================================================================
    constexpr double CONFIANCA_MINIMA_FACIAL = 0.75;
    constexpr int INTERVALO_DETECCAO_MOVIMENTO = 8500; // em millisegundos
    constexpr int INTERVALO_DETECCAO_FACES = 3000; // em millisegundos
    constexpr int INTERVALO_DETECCAO_FACIAL = 2000; // em millisegundos
    constexpr int INTERVALO_DETECCAO_PARADO = 2000; // em millisegundos
    constexpr int INTERVALO_DETECCAO_BARULHO = 10000;
    constexpr double DURACAO_MAXIMA_VIDEO = 60.0 * 50; // em segundos. Por lei: 50 minutos
    constexpr int MAX_RECONHECIMENTOS_EM_BUFFER = 15;
    constexpr int MELHORAR_BRILHO = -35;   // (100 = 100%)
    constexpr double MELHORAR_CONTRASTE = 1.35;  // (1.0 = 100%)
    constexpr int DESLOCAMENTO = 15;
    constexpr int DESLOCAMENTO_MAXIMO_X = 6;
    constexpr int DESLOCAMENTO_MAXIMO_Y = 6;
    constexpr int INDICE_PARA_RECONHECIMENTO = 65;
    constexpr int MARGEM_IMAGEM = 1;

    // =================================================================
    // @note Para a analise da face
    // =================================================================
    constexpr double MAX_HEAD_PITCH_DEGREES = 20.0;
    constexpr double MAX_HEAD_YAW_DEGREES = 20.0;
    constexpr double MAX_HEAD_ROLL_DEGREES = 20.0;
    constexpr int REFLEXOS_AREA_MINIMA = 150;
    constexpr int REFLEXOS_AREA_MAXIMA = 3500;
    constexpr double REFLEXOS_SOLIDEZ_MAXIMA = 0.45;

    // =================================================================
    // Validação de Fundo e Iluminação
    // =================================================================
    constexpr double MAX_BACKGROUND_STDDEV = 15.0; // Desvio padrão máximo para fundo uniforme
    constexpr double MIN_BACKGROUND_INTENSITY = 180.0; // Intensidade média mínima para fundo claro
    constexpr double MAX_FACE_LIGHTING_STDDEV = 40.0; // Desvio padrão máximo para iluminação facial
    constexpr double MAX_FACE_HALVES_INTENSITY_DIFF = 20.0; // Diferença máxima de brilho entre as metades do rosto


    // =================================================================
    // Caminhos de Modelos e Classificadores
    // =================================================================
    const std::string HAARCASCADE_EYE_TREE_EYEGLASSES = "./models/haarcascade_eye_tree_eyeglasses.xml";
    const std::string HAARCASCADE_EYE = "./models/haarcascade_eye.xml";
    const std::string HAARCASCADE_MOUTH = "./models/haarcascade_mcs_mouth.xml";

    // =================================================================
    // Caminhos de Arquivos
    // =================================================================
    const std::string FILENAME_LOG = "gCV.log";
    const std::string FILENAME_VIDEO = "bio-video.avi";
    const std::string FILENAME_AUDIO = "bio-audio.wav";

}

#endif // CONFIG_HPP
