#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "preProcessing/SDL_Function/sdlFunction.h"

#define FACTOR 10


/***************************************************************
 *  Function applyContrastFilter :
 *
 *  Apply a contract filter to the input image
 *
 *  @input :
 *      - img (SDL_Surface*) : input image
 *
 *  @output :
 *      - (SDL_Surface*) : output image
***************************************************************/

SDL_Surface* applyContrastFilter(SDL_Surface* img){

    SDL_Surface *outImg = SDL_ConvertSurfaceFormat
                            (
                                img,
                                SDL_PIXELFORMAT_ABGR8888,
                                0
                            );

    Uint32* pixels = (Uint32*) outImg->pixels;


    for (int y = 0; y < outImg->h; y++)
    {
        for (int x = 0; x < outImg->w; x++)
        {
            for (int i = 0; i < FACTOR; i++)
            {
                int pxVal = getPixelGrayScale(pixels[y * outImg->w + x]);

                if(pxVal >= i * (255/FACTOR) &&
                    pxVal <= (i + 1) * (255 / FACTOR))
                {
                    int newPxVal = (i + 1) * (255 / FACTOR);
                    //printf("px Val = %d\n", pxVal);
                    Uint32 newPx = SDL_MapRGBA
                        (
                            outImg->format,
                            newPxVal,
                            newPxVal,
                            newPxVal,
                            255
                        );



                    pixels[y * outImg->w + x] = newPx;

                    break;
                }
            }
        }
    }

    return outImg;
}