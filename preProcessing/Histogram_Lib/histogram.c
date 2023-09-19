#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#define HISTO_W_BAR 5

int* findHistogram(SDL_Surface* img){
    int histogram[256];

    for (int x = 0; x < img->w; x++)
    {
        for (int y = 0; y < img->h; y++)
        {   

            //get pixel
            Uint32 pixel = getpixel(img, x, y);

            //get color of the pixel
            Uint8 r, ignore_g, ignore_b, ignore_a;
            SDL_GetRGBA(pixel, img->format, r, ignore_g, ignore_b, ignore_a);

            histogram[r]++;
        }        
    }
    
    return histogram;
}


SDL_Surface* createHistogramImg(int* histogram){

    //Get histogram max value for the height
    int maxVal = histogram[0];

    for (int i = 0; i < 256; i++)
    {
        if(maxVal < histogram[i]){
            maxVal = histogram[i];
        }
    }    

    SDL_Surface *outputSurface = SDL_CreateRGBSurface(0, 256*HISTO_W_BAR, maxVal, 32, 0, 0, 0, 0);
    Uint32 pixel = SDL_MapRGBA(outputSurface->format, 255, 0, 0, 0);


    for (int x = 0; x < 256; x++)
    {
        outputSurface = fillRect(outputSurface, x*HISTO_W_BAR, x*HISTO_W_BAR+HISTO_W_BAR, 0, histogram[x], pixel);
    }

    return outputSurface;
}