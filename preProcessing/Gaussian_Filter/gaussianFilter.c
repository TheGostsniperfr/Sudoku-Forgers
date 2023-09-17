#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../SDL_Function/sdlFunction.h"


//Size of the gaussian matrix. (Must be a odd number !!)
#define KERNEL_SIZE 3

//Standard deviation of the gaussian matrix
#define KERNEL_SIGMA 3


double** createGaussianKernel(){
    //sum of all number in the kernel is equal to 1


    // Create dynamique array
    double** kernel = (double**)malloc(KERNEL_SIZE * sizeof(double*));
    for (int i = 0; i < KERNEL_SIZE; i++) {
        kernel[i] = (double*)malloc(KERNEL_SIZE * sizeof(double));
    }


    double sum = 0.0;
    for (int x = -KERNEL_SIZE/2; x <= KERNEL_SIZE/2; x++) {
        for (int y = -KERNEL_SIZE/2; y <= KERNEL_SIZE/2; y++) {
            kernel[x + KERNEL_SIZE/2][y + KERNEL_SIZE/2] = exp(-((x * x + y * y) / (2.0 * KERNEL_SIGMA * KERNEL_SIGMA)));
            sum += kernel[x + KERNEL_SIZE/2][y + KERNEL_SIZE/2];
        }
    }

    // Kernel normalization
    for (int x = 0; x < KERNEL_SIZE; x++) {
        for (int y = 0; y < KERNEL_SIZE; y++) {
            kernel[x][y] /= sum;
        }
    }

    return kernel;
}

//debug function to print the gaussian kernel
void printKernel(double** kernel){
    for (int y = 0; y < KERNEL_SIZE; y++)
    {
        for (int x = 0; x < KERNEL_SIZE; x++)
        {
            printf(" %f", kernel[y][x]);
        }
        printf("\n");
    }    
}



SDL_Surface* applyGaussianFilter(SDL_Surface* inputSurface){

    double** kernel = createGaussianKernel();    
                 
    SDL_Surface *outputSurface = SDL_CreateRGBSurface(0, inputSurface->w, inputSurface->h, 32, 0, 0, 0, 0);

    // Copie de l'image d'entrée dans l'image de sortie en utilisant des boucles
    for (int y = 0; y < inputSurface->h; ++y) {
        for (int x = 0; x < inputSurface->w; ++x) {
            //get pixel 
            Uint32 pixel = getpixel(inputSurface, x, y);

            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, inputSurface->format, &r, &g, &b, &a);

            double newR = 0.0, newG = 0.0, newB = 0.0;
            for (int i = 0; i < KERNEL_SIZE; i++) {
                for (int j = 0; j < KERNEL_SIZE; j++) {
                    int newX = x + i ;
                    int newY = y + j ;
                    if (newX >= 0 && newX < inputSurface->w && newY >= 0 && newY < inputSurface->h) {

                        
                        Uint32 newPixel = getpixel(inputSurface, newX, newY);
                        

                        Uint8 nr, ng, nb, na;
                        SDL_GetRGBA(newPixel, inputSurface->format, &nr, &ng, &nb, &na);
                        newR += kernel[i][j] * nr;
                        newG += kernel[i][j] * ng;
                        newB += kernel[i][j] * nb;
                    }
                }
            }

            Uint32 newPixel = SDL_MapRGBA(inputSurface->format, (Uint8)newR, (Uint8)newG, (Uint8)newB, a);
            

            //set new pixel
            *((Uint32*)((Uint8*)outputSurface->pixels + y * outputSurface->pitch + x * sizeof(Uint32))) = newPixel;
        }
    }

    return outputSurface;


}


