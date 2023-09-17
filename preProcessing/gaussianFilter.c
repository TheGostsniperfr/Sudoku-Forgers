#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "preProcessing.h"


//Size of the gaussian matrix. (Must be a odd number !!)
#define KERNEL_SIZE 3

//Standard deviation of the gaussian matrix
#define KERNEL_SIGMA 1


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

SDL_Surface* applyGaussianFilter(SDL_Surface* img){

    double** kernel = createGaussianKernel();    
    SDL_Surface *outputImage = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0, 0);
                 


    for (int x = 0; x < img->w; x++) {
        for (int y = 0; y < img->h; y++) {

            //get current pixel
            Uint32 pixel = *((Uint32*)img->pixels + y * img->pitch / sizeof(Uint32) + x);

            Uint8 r, g, b, a;

            SDL_GetRGBA(pixel, img->format, &r, &g, &b, &a);

            double newR = 0.0, newG = 0.0, newB = 0.0;
            for (int i = 0; i < KERNEL_SIZE; i++) {
                for (int j = 0; j < KERNEL_SIZE; j++) {

                    //get neighbor pixel
                    int newX = x + i - KERNEL_SIZE / 2;
                    int newY = y + j - KERNEL_SIZE / 2;

                     //check if the neighbor pixel is valid
                    if (newX >= 0 && newX < img->w && newY >= 0 && newY < img->h) {

                        Uint32 newPixel = *((Uint32*)img->pixels + newY * img->pitch / sizeof(Uint32) + newX);

                        Uint8 nr, ng, nb, na;
                        SDL_GetRGBA(newPixel, img->format, &nr, &ng, &nb, &na);

                        //Apply gaussian filter to a new pixel
                        newR += kernel[i][j] * nr;
                        newG += kernel[i][j] * ng;
                        newB += kernel[i][j] * nb;
                    }
                }
            }

            //Insert the new pixel in the output image
            Uint32 newPixel = SDL_MapRGBA(img->format, (Uint8)newR, (Uint8)newG, (Uint8)newB, a);
            ((Uint32 *)outputImage->pixels)[y * img->w + x] = newPixel;
        }
    }

    return outputImage;


}

/*
int main(){
    

    SDL_Surface* img = loadImg("../data/sudoku_default_images/image_01.jpeg");

    applyGaussianFilter(img);

    saveImg(img, "gaussianOutput.jpeg");

}*/
