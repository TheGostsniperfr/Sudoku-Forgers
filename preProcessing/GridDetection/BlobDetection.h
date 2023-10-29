#ifndef BLOBDETECTION_H
#define BLOBDETECTION_H 

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "../Pixel/pixel.h"
#include <err.h>

typedef struct Point{
	int x;
	int y;
}Point;

void Blob(char *path);

#endif