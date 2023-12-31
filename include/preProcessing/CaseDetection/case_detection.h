#ifndef CASE_DETECTION_H
#define CASE_DETECTION_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/Matrix/mat.h"

typedef struct GridCell {
	int positionX;
	int positionY;
	SDL_Surface* image;
	int isDigit; //1 for digit, 0 for empty cell
	int label; //Digit associate of the image
}GridCell;

double* Fill_Matrix_Division(int size, int* points);

SDL_Surface* Homography_Transform_Division
(
	SDL_Surface* image,
	int size,
	int* points
);

GridCell* CaseDetection(SDL_Surface* image, SDL_Surface* test);

#endif