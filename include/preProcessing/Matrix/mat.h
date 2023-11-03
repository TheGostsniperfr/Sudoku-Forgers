#ifndef MAT_H
#define MAT_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

void Print_Mat(double* mat, int height, int width);
void Mult_Mat(double* matA, double* matB, int heightA, int widthA, int widthB,
    double* result);
void Add_Mat(double* matA, double* matB, int height, int width);
void Sub_Mat(double* matA, double* matB, int height, int width);
void Transpose_Mat(double* mat, double* result, int height, int width);
void Trans_Mat(double* mat, double* temp, long size, long a, long b, int cols);
double Det_Mat(double* mat, int size, int cols);
void Adj_Mat(double* mat, double* adj, int size);
int Inverse_Mat(double* mat, double* inverse, int size);

#endif