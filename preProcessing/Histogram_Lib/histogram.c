#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#define HISTO_W_BAR 50

int* findHistogram(SDL_Surface* img){
    int* histogram = (int*)malloc(256 * sizeof(int)) ;

    for (int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }
    

    Uint32* pixels = (Uint32*) img->pixels;

    for (int x = 0; x < img->w; x++)
    {
        for (int y = 0; y < img->h; y++)
        {   

            //get pixel
            Uint32 pixel = pixels[y * img->w + x];

            //get color of the pixel
            Uint8 r, ignore_g, ignore_b, ignore_a;
            SDL_GetRGBA(pixel, img->format, &r, &ignore_g, &ignore_b, &ignore_a);

            histogram[r]++;
        }        
    }
    
    /*
    int wh = img->w*img->h;

    for (int i = 0; i < 256; i++)
    {
        histogram[i] = histogram[i] / wh;
    }
    */


    return histogram;
}


SDL_Surface* createHistogramImg(int* histogram) {
    // Get histogram max value for the height
    int maxVal = histogram[0];

    for (int i = 0; i < 256; i++) {
        if (maxVal < histogram[i]) {
            maxVal = histogram[i];
        }
    }

    SDL_Surface* outputSurface = SDL_CreateRGBSurfaceWithFormat(0, 256 * HISTO_W_BAR, maxVal, 32, SDL_PIXELFORMAT_ABGR8888);
    Uint32 pixel = SDL_MapRGBA(outputSurface->format, 255, 0, 0, 0);

    for (int x = 0; x < 256; x++) {
        SDL_Rect rect;
        rect.x = x * HISTO_W_BAR;
        rect.y = maxVal - histogram[x]; 
        rect.w = HISTO_W_BAR;
        rect.h = histogram[x];

        SDL_FillRect(outputSurface, &rect, pixel);
    }

    return outputSurface;
}