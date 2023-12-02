#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "preProcessing/Noise_Level/NoiseLevel.h"

#define NB_IMAGE_PART 32


/***************************************************************
 *  Function applyAdaptiveThreshold :
 *
 *  Apply a adaptive threshold to the input image
 *
 *  @input :
 *      - inputImg (SDL_Surface*) : input image
 *
 *  @output :
 *      - (SDL_Surface*) : output image
***************************************************************/

SDL_Surface* applyAdaptiveThreshold(SDL_Surface* inputImg) {
    SDL_Surface* outputImg = SDL_ConvertSurfaceFormat
        (
            inputImg,
            SDL_PIXELFORMAT_ABGR8888,
            0
        );

    Uint32* outPixels = (Uint32*) outputImg->pixels;
    Uint32* inPixels = (Uint32*) inputImg->pixels;

    int* integral =
        calloc(inputImg->w * inputImg->h, sizeof(int));

    int sum = 0;

    for (int y = 0; y < inputImg->h; y++)
    {
        sum += getPixelGrayScale(inPixels[y * inputImg->w]);
        integral[y] = sum;
    }


    //calcul of integral
    for (int x = 1; x < inputImg->w; x++)
    {
        sum = 0;
        for (int y = 0; y < inputImg->h; y++)
        {
            sum += getPixelGrayScale(inPixels[y * inputImg->w + x]);



            integral[x * inputImg->h + y] =
                            integral[(x - 1) * inputImg->h + y] + sum;

        }
    }

    //set the size of the window
    int windowSize = (inputImg->w > inputImg->h) ?
                        inputImg->w : inputImg->h / NB_IMAGE_PART;


    //calcul noise level of the image

    int noiseLevel = calculNoiseLevel(inputImg);

    //printf("Noise lvl = %d\n", noiseLevel);

    //apply threshold
    double threshold = 0.15;
    if(noiseLevel < 20){
        threshold = 0.1;
    }
    if(noiseLevel >= 2 && noiseLevel < 100){
        threshold = 0.15;
    }
    if(noiseLevel > 100 && noiseLevel <= 500){
        threshold = 0.2;
    }
    if(noiseLevel > 500){
        threshold = 0.5;
    }

    int value = 0;
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    Uint32 whitePx = SDL_MapRGB(outputImg->format, 255, 255, 255);
    Uint32 blackPx = SDL_MapRGB(outputImg->format, 0, 0, 0);

    for (int x = 0; x < inputImg->w; x++)
    {
        for (int y = 0; y < inputImg->h; y++)
        {

            //calcul x1
            x1 = x - windowSize;
            x1 = (x1 < 1) ? 1 : x1;

            //calcul x2
            x2 = x + windowSize;
            x2 = (x2 > (inputImg->w - 1)) ?
                    (inputImg->w - 1) : x2;

            //calcul y1
            y1 = y - windowSize;
            y1 = (y1 < 1) ? 1 : y1;

            //calcul y2
            y2 = y + windowSize;
            y2 = (y2 > (inputImg->h - 1)) ?
                    (inputImg->h - 1) : y2;

            value = (x2 - x1) * (y2 - y1);

            sum =   integral[(x1 - 1) * inputImg->h + (y1 - 1)] -
                    integral[(x1 - 1) * inputImg->h + y2] -
                    integral[x2 * inputImg->h + (y1 - 1)] +
                    integral[x2 * inputImg->h + y2];

            int pxVal = getPixelGrayScale(inPixels[y * inputImg->w + x]);

            if (pxVal * value < sum * (1.0 - threshold)){
                outPixels[y * outputImg->w + x] = blackPx;
            }
            else{
                outPixels[y * outputImg->w + x] = whitePx;
            }
        }
    }

    free(integral);

    return outputImg;
}