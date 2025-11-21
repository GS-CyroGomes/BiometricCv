#pragma once

extern "C" {
    void InitializeValidator();

    bool ValidateFinalPhoto(
        const unsigned char* imageData, 
        int imageSize, 
        char* failureReason, 
        int reasonSize
    );
}
