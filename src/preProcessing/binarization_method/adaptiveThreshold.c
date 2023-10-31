#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define KERNEL_SIZE 3
#define SIGMA 0.5


typedef struct VarMap{
    int width;
    int height;
    double** data;
} VarMap;


VarMap* initVarMap(int w, int h){

    double** d = calloc(h, sizeof(double*));

    for (int y = 0; y < h; y++)
    {
        d[y] = calloc(w, sizeof(double));
    }

    VarMap* varMap = (VarMap*)malloc(sizeof(VarMap));

    if(varMap != NULL)
    {
        varMap->width = w;
        varMap->height = h;
        varMap->data = d;
    }

    return varMap;
}

void destroyVarMap(VarMap* varMap){
    for (int y = 0; y < varMap->height; y++)
    {
        free(varMap->data[y]);
    }

    free(varMap->data);
}


VarMap* createVarMap(SDL_Surface* img){

    VarMap* varMap = initVarMap(img->w, img->h);


    Uint32* pixels = img->pixels;

    int k = KERNEL_SIZE / 2;

    for (int y = 0; y < img->h; y++) {
        for (int x = 0; x < img->w; x++) {
            double sum = 0.0;
            double sumOfSquares = 0.0;

            for (int j = -k; j <= k; j++) {
                for (int i = -k; i <= k; i++) {
                    int nx = x + i;
                    int ny = y + j;

                    if (nx >= 0 && nx < img->w && ny >= 0 && ny < img->h) {
                        int pxValue = getPixelGrayScale(pixels[ny * img->w + nx]);
                        sum += pxValue;
                        sumOfSquares += pxValue * pxValue;
                    }
                }
            }

            double mean = sum / (KERNEL_SIZE * KERNEL_SIZE);
            double variance = (sumOfSquares / (KERNEL_SIZE * KERNEL_SIZE)) - (mean * mean);

            varMap->data[y][x] = variance;
        }
    }

    return varMap;
}


SDL_Surface* applyAdaptiveThreshold(SDL_Surface* inputImg) {


    VarMap* varMap = createVarMap(inputImg);


    SDL_Surface *outputImg = SDL_ConvertSurfaceFormat(
        inputImg,
        SDL_PIXELFORMAT_ABGR8888,
        0
    );

    Uint32* pixelsIn = inputImg->pixels;
    Uint32* pixelsOut = outputImg->pixels;

    Uint32 white = SDL_MapRGBA(inputImg->format, 255, 255, 255, 255);
    Uint32 black = SDL_MapRGBA(inputImg->format, 0, 0, 0, 255);

    for (int y = 0; y < varMap->height; y++) {
        for (int x = 0; x < varMap->width; x++) {
            double threshold = SIGMA * varMap->data[y][x];

            if (getPixelGrayScale(pixelsIn[y * inputImg->w + x]) > threshold) {
                pixelsOut[y * inputImg->w + x] = white;
            } else {
                pixelsOut[y * inputImg->w + x] = black;
            }
        }
    }



    destroyVarMap(varMap);

    return outputImg;
}
