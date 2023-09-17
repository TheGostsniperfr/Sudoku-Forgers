#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

SDL_Surface* applyGrayScaleFilter(SDL_Surface* inputSurface){

    SDL_Surface *outputSurface = SDL_CreateRGBSurface(0, inputSurface->w, inputSurface->h, 32, 0, 0, 0, 0);

    for (int x = 0; x < inputSurface->w; x++)
    {
        for (int y = 0; y < inputSurface->h; y++)
        {
            //get pixel
            Uint32 pixel = getpixel(inputSurface, x, y);

            //get color of the pixel
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, inputSurface->format, &r, &g, &b, &a);

            //set the new pixel (gray = 0.299 * R + 0.587 * G + 0.114 * B)
            Uint8 gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);

            Uint32 newPixel = SDL_MapRGBA(inputSurface->format, gray, gray, gray, a);
            

            //set new pixel
            ((Uint32 *)outputSurface->pixels)[y * inputSurface->w + x] = newPixel;
        }
        
    }

    return outputSurface;    
}
