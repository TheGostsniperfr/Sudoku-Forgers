#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "preProcessing/SDL_Function/sdlFunction.h"


/***************************************************************
 *  Function applyIlluminationFilter :
 *
 *  Apply a illumination filter to the input image
 *
 *  @input :
 *      - img (SDL_Surface*) : input image
 *
 *  @output :
 *      - (SDL_Surface*) : output image
***************************************************************/

SDL_Surface* applyIlluminationFilter(SDL_Surface* img){

    SDL_Surface *outImg = SDL_ConvertSurfaceFormat
                            (
                                img,
                                SDL_PIXELFORMAT_ABGR8888,
                                0
                            );

    Uint32* inPixels = (Uint32*) img->pixels;
    Uint32* outPixels = (Uint32*) outImg->pixels;

    int maxPxVal = 0;

    //find max val
    for (int x = 0; x < img->w; x++)
    {
        for (int y = 0; y < img->h; y++)
        {
            int currentPxVal = getPixelGrayScale(inPixels[y * img->w + x]);
            if(currentPxVal > maxPxVal){
                maxPxVal = currentPxVal;
            }
        }
    }


    //Image Normalization


    for (int x = 0; x < outImg->w; x++)
    {
        for (int y = 0; y < outImg->h; y++)
        {
            int newPxVal = getPixelGrayScale(inPixels[y * img->w + x]
                                                        / maxPxVal * 255);

            Uint32 newPx = SDL_MapRGBA
                        (
                            outImg->format,
                            newPxVal,
                            newPxVal,
                            newPxVal,
                            255
                        );

            outPixels[y * outImg->w + x] = newPx;
        }
    }

    return outImg;
}