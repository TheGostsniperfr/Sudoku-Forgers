#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#include "../Pixel/pixel.h"
#include "../Corner_Finder/coins_detecter.h"
#include "../Matrix/mat.h"

double* Fill_Matrix(SDL_Surface* image, int size);
SDL_Surface* Homography_Transform(SDL_Surface* image, int size);

#endif