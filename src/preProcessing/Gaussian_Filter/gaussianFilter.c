#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "preProcessing/SDL_Function/sdlFunction.h"


//Size of the gaussian matrix. (Must be a odd number !!)
#define KERNEL_SIZE 3

//Standard deviation of the gaussian matrix
#define KERNEL_SIGMA 3

/***************************************************************
 *  Function createGaussianKernel:
 *
 *  Create a gaussian kernel with defined KERNEL_SIZE and KERNEL_SIGMA
 *
 *  @output :
 *      - (double**) : matrix of the lernel
***************************************************************/
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

/***************************************************************
 *  Function categorical_cross_entropy_loss:
 *
 *  Debug function to print the gaussian kernel
 *
 *  @input :
 *      - kernel (double**) : Gaussian kernel
***************************************************************/
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


/***************************************************************
 *  Function applyGaussianFilter:
 *
 *  Apply the gaussian filter on an image
 *
 *  @input :
 *      - inputSurface (SDL_Surface*) : img to apply the gaussian filter
 *  @output :
 *      - outputSurface (SDL_Surface*) : output image with the gaussian filter applied
***************************************************************/
SDL_Surface* applyGaussianFilter(SDL_Surface* inputSurface){

    double** kernel = createGaussianKernel();

    SDL_Surface *outputSurface = SDL_ConvertSurfaceFormat(inputSurface, SDL_PIXELFORMAT_ABGR8888, 0);

    Uint32* pixels = (Uint32*) outputSurface->pixels;

    for (int y = 0; y < outputSurface->h; ++y) {
        for (int x = 0; x < outputSurface->w; ++x) {
            //get pixel
            Uint32 pixel = pixels[y * outputSurface->w + x];

            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, inputSurface->format, &r, &g, &b, &a);

            double newR = 0.0, newG = 0.0, newB = 0.0;
            for (int i = 0; i < KERNEL_SIZE; i++) {
                for (int j = 0; j < KERNEL_SIZE; j++) {
                    int newX = x + i ;
                    int newY = y + j ;
                    if (newX >= 0 && newX < outputSurface->w && newY >= 0 && newY < outputSurface->h) {


                        Uint32 newPixel = pixels[newY * outputSurface->w + newX];


                        Uint8 nr, ng, nb, na;
                        SDL_GetRGBA(newPixel, outputSurface->format, &nr, &ng, &nb, &na);
                        newR += kernel[i][j] * nr;
                        newG += kernel[i][j] * ng;
                        newB += kernel[i][j] * nb;
                    }
                }
            }

            Uint32 newPixel = SDL_MapRGBA(outputSurface->format, (Uint8)newR, (Uint8)newG, (Uint8)newB, a);


            //set new pixel
            pixels[y * outputSurface->w + x] = newPixel;
        }
    }
    free(kernel);

    return outputSurface;


}


