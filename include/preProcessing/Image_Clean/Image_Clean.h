#ifndef NOISE_DETECTION_H
#define NOISE_DETECTION_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

#include "preProcessing/BlobDetection/BlobDetection.h"
#include "preProcessing/CaseDetection/case_detection.h"

void reversedBW(SDL_Surface* image);

void clean_edge(SDL_Surface* image);

void clean_all(SDL_Surface* image);

void Image_Clean(GridCell* images);

#endif