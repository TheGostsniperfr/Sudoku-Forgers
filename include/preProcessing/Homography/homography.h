#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/Matrix/mat.h"
#include "preProcessing/Corner_Finder/find_corners.h"


double* Fill_Matrix(SDL_Surface* image, int size);
SDL_Surface* Homography_Transform(SDL_Surface* image, int size);

#endif