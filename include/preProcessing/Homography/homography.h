#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/Matrix/mat.h"


double* Fill_Matrix(int size, int* points);
SDL_Surface* Homography_Transform(SDL_Surface* image, int size, int* points);

#endif