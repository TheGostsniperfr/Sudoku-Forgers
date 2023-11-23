#ifndef NETWORK_HANDLE_H
#define NETWORK_HANDLE_H

#include "GUI/handleUtils.h"

#include "neuralNetwork/network_Utils/struct.h"


typedef struct Option
{
    char* flag;
    int(*action)
        (
            int argc,
            char* argv[],
            NeuralNetwork* net,
            Flag* flag
        );

} Option;

int handleXorTrain(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);

int handleDigitsTrain(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);

int handleLoad(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);

int handleTestXor(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags  );

int handleTestDigit(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags  );

int handlePrintHelp(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);

int handleGetImgFromMnist(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);


#endif