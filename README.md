# Biometric CV

Este é um aplicativo de desktop para captura de dados biométricos faciais e de áudio. O sistema utiliza uma câmera para detectar um rosto, verifica a qualidade da imagem e do ambiente e, em seguida, grava um pequeno clipe de vídeo e áudio do usuário.

O projeto foi desenvolvido em C++ e utiliza um padrão de máquina de estados para gerenciar os diferentes estágios do processo de captura, como "Procurando Rosto", "Verificando Ambiente", "Capturando Biometria" e estados de "Sucesso" ou "Falha".

## Principais Características

- **Detecção de Rosto em Tempo Real**: Utiliza modelos do OpenCV e dlib para localizar rostos na imagem da câmera.
- **Análise de Qualidade**: Executa verificações para garantir que o rosto está bem posicionado e que as condições de iluminação são adequadas.
- **Captura de Áudio e Vídeo**: Grava simultaneamente vídeo e áudio durante a fase de captura biométrica.
- **Arquitetura de Servidor**: Inclui servidores de socket e streaming, sugerindo que o aplicativo pode ser controlado ou monitorado remotamente.
- **Interface de Usuário Simples**: Exibe o feed da câmera e mensagens de status em uma janela do OpenCV.

## Pilha de Tecnologia

- **Linguagem**: C++ (C++17 ou superior)
- **Sistema de Build**: CMake
- **Bibliotecas Principais**:
  - **OpenCV**: Para processamento de vídeo, detecção de rosto e interface de usuário.
  - **PortAudio**: Para captura de áudio multiplataforma.
  - **libsndfile**: Para manipulação de arquivos de áudio.
  - **dlib**: (Inferido pelo modelo `shape_predictor_68_face_landmarks.dat`) Para detecção de pontos de referência faciais.

## Como Compilar e Executar

### Pré-requisitos

- **CMake**: Versão 3.10 ou superior.
- **Um compilador C++**: Como GCC (MinGW-w64 no Windows), Clang ou MSVC.
- As bibliotecas (OpenCV, PortAudio) estão incluídas no repositório (`vendor/` e `libs/`), então não devem ser necessárias instalações adicionais.

### Passos para Compilar

O projeto inclui um script de compilação para facilitar o processo no Windows.

1.  **Usando o script `build.bat`**:
    - Abra um terminal (prompt de comando ou PowerShell).
    - Navegue até a raiz do projeto.
    - Execute o script:
      ```bash
      .\build.bat
      ```

2.  **Manualmente com CMake (alternativa)**:
    ```bash
    # 1. Crie um diretório de build
    mkdir build
    cd build

    # 2. Execute o CMake para configurar o projeto
    # Se estiver usando MinGW, você pode precisar especificar o gerador
    cmake .. -G "MinGW Makefiles"

    # 3. Compile o projeto
    cmake --build .
    ```

### Como Executar

Após a compilação bem-sucedida, o executável estará localizado no diretório `build/`.

1.  Navegue até o diretório `build`:
    ```bash
    cd build
    ```
2.  Execute o aplicativo:
    ```bash
    .\biometric_capture.exe
    ```

## Estrutura do Projeto

```
├── build/              # Diretório de saída da compilação (gerado)
├── include/            # Cabeçalhos de bibliotecas de terceiros (PortAudio)
├── libs/               # Arquivos .dll de bibliotecas de terceiros
├── models/             # Modelos de machine learning para detecção facial
├── src/                # Código-fonte principal do aplicativo
│   ├── states/         # Implementação dos diferentes estados da aplicação
│   └── ...
├── vendor/             # Código-fonte ou cabeçalhos de dependências (OpenCV)
├── build.bat           # Script de compilação para Windows
├── CMakeLists.txt      # Arquivo de configuração do CMake
└── README.md           # Este arquivo
```
