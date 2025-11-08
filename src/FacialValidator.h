#pragma once

#ifdef FACIALVALIDATOR_EXPORTS
#define FACIALVALIDATOR_API __declspec(dllexport)
#else
#define FACIALVALIDATOR_API __declspec(dllimport)
#endif

extern "C" {
    /**
     * @brief Inicializa os classificadores e modelos necessários para a validação.
     * Deve ser chamada uma vez antes de qualquer outra função.
     */
    FACIALVALIDATOR_API void InitializeValidator();

    /**
     * @brief Valida uma imagem final de acordo com os critérios da Portaria Nº 968.
     * 
     * @param imageData Ponteiro para os dados da imagem em formato de bytes (ex: JPEG).
     * @param imageSize O tamanho do buffer de dados da imagem.
     * @param failureReason Um buffer de saída para receber a mensagem de falha (UTF-8).
     * @param reasonSize O tamanho do buffer failureReason.
     * @return true se a imagem for válida, false caso contrário.
     */
    FACIALVALIDATOR_API bool ValidateFinalPhoto(
        const unsigned char* imageData, 
        int imageSize, 
        char* failureReason, 
        int reasonSize
    );
}
