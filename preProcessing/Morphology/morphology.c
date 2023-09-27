#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#define EROSION_RANGE 1

//return 1 if the nulber is valid, else 0
int isValid(int x, int y, int w, int h){
    if(x>= 0 && x <= w){
        if(y>= 0 && y <= h){
            return 1;
        }
    }
    return 0;
}


SDL_Surface* erosion(SDL_Surface* img){
    Uint32* pixels = img->pixels;

    for (int y = 0; y < img->h; y++)
    {
        for (int x = 0; x < img->w; x++)
        {
            
        }
        
    }
    
}



