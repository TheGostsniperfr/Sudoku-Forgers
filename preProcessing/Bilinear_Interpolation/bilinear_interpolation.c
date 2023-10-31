#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#include "../Pixel/pixel.h"


SDL_Surface* Bilinear_Interpolation(SDL_Surface* image, int output_width, int output_height)
{
    SDL_Surface* output_image = SDL_CreateRGBSurface(0, output_width, output_height, 32,0,0,0,0);

    double input_width = image->w;
    double input_height = image->h;

    for (int i = 0; i < output_height; i++) 
    {
        for (int j = 0; j < output_width; j++)
        {
            
        }
    }

    return output_image;
}


/*
Bilinear interpolation

x, y points of the surface that will be returned

find i and j that are the coord of x, y on the input surface

c00 = i, j
c10 = i+1, j
c01 = i, j+1
c11 = i+1, j+1

--> ceil and floor 'cause points are in float mode

interpolation linéaire sur ces points pour trouver la couleur de x, y

*/