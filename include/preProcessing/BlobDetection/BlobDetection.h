#ifndef BLOBDETECTION_H
#define BLOBDETECTION_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>

typedef struct {
    double x, y;
} Pointx_y;

void Fill
(
    SDL_Surface *src,
    Uint32* pixels,
    int x,
    int y,
    int* blob,
    int lim
);

SDL_Surface* Blob
(
    SDL_Surface* src,
    int* size_max
);

double Distance
(
    Pointx_y p1, 
    Pointx_y p2
);

int isSquare_Blob
(
    Pointx_y p1, 
    Pointx_y p2, 
    Pointx_y p3, 
    Pointx_y p4
);

SDL_Surface* Remove_Blob
(
    SDL_Surface* image,
    SDL_Surface* image_blob
);


#endif