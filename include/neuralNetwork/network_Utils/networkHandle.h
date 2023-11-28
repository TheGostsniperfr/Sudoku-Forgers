#ifndef NETWORK_HANDLE_H
#define NETWORK_HANDLE_H

#include "GUI/handleUtils.h"

#include "neuralNetwork/network_Utils/struct.h"


typedef struct OptionNet
{
    char* flag;
    void* (*action)
        (
            int argc,
            char* argv[],
            NeuralNetwork* net,
            Flag* flag
        );

} OptionNet;

void* handleXorTrain(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);

void* handleDigitsTrain(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);

void* handleLoad(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);

void* handleTestXor(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags  );

void* handleTestDigit(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags  );

void* handlePrintNetHelp(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);

void* handleGetImgFromMnist(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags);


#endif