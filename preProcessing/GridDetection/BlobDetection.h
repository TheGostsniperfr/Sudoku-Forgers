#include <SLD2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "../Rotation/rotation.h"
#include <err.h>

#ifndef BLOBDETECTION_H
#define BLOBDETECTION_H 

typedef struct Point{
	int x;
	int y;
}Point;

void Blob(char *path);

#endif