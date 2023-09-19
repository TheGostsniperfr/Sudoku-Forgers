#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

SDL_Surface* applyGrayScaleFilter(SDL_Surface* inputSurface){

    SDL_Surface *outputSurface = SDL_ConvertSurfaceFormat(inputSurface, SDL_PIXELFORMAT_ABGR8888, 0);

    Uint32* pixels = (Uint32*) outputSurface->pixels;


    for (int x = 0; x < outputSurface->w; x++)
    {
        for (int y = 0; y < outputSurface->h; y++)
        {
            //get pixel
            Uint32 pixel = pixels[y * outputSurface->w + x];

            //get color of the pixel
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, inputSurface->format, &r, &g, &b, &a);

            //set the new pixel (gray = 0.299 * R + 0.587 * G + 0.114 * B)
            Uint8 gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);

            Uint32 newPixel = SDL_MapRGBA(inputSurface->format, gray, gray, gray, a);
            

            //set new pixel
            pixels[y * outputSurface->w + x] = newPixel;
        }
        
    }

    return outputSurface;    
}
