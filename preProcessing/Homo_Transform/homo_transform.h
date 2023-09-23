#ifndef HOMO_TRANSFORM_H
#define HOMO_TRANSFORM_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

int* coord_detection(SDL_Surface* surface);
int** create_homo_matrix(int* coordonate);
SDL_Surface* application(SDL_Surface* surface);


#endif