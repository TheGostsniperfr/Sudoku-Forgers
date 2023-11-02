#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define NOISE_WINDOW_SIZE 3
#define NOISE_THRESHOLD 0.5

int calculNoiseLevel(SDL_Surface* img){
    Uint32* pixels = (Uint32*) img->pixels;
    int k = NOISE_WINDOW_SIZE / 2;
    int totalNoise = 0;

    for (int x = 0; x < img->w; x++)
    {
        for (int y = 0; y < img->h; y++)
        {
            //calcul the mean value of neightbours pixels
            double mean = 0;

            for (int i = x - k; i <= x + k; i++)
            {
                for (int j = y - k; j <= y + k; j++)
                {
                    //check valid coordonate
                    if (i >= 0 && i < img->w &&
                            j >= 0 && j < img->h)
                    {
                        mean += getPixelGrayScale(pixels[j * img->w + i]);
                    }
                }
            }

            mean /= 9;

            double val = 1 -
                (getPixelGrayScale(pixels[y * img->w + x]) / mean);

            val = fabs(val);

            if(val > NOISE_THRESHOLD){
                totalNoise++;
            }
        }
    }

    return totalNoise;
}