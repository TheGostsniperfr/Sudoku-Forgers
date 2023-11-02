#ifndef DIVISION_BY_9_H
#define DIVISION_BY_9_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#include "../Pixel/pixel.h"
#include "../Matrix/mat.h"

double* Fill_Matrix_Division(int size, int* points);
SDL_Surface* Homography_Transform_Division(SDL_Surface* image, int size,
	int* points);
SDL_Surface** Division9(SDL_Surface* image);

#endif