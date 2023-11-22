#ifndef MNIST_UTILS_H
#define MNIST_UTILS_H

#include "SDL2/SDL.h"


#include "GUI/handleUtils.h"
#include "neuralNetwork/network_Utils/struct.h"



ImgContainer* getImageFromMnist(int index, Flag* flags);
void freeImgContainer(ImgContainer* imgContainer);
ImgContainer* getDataSet(int batchSize);

#endif