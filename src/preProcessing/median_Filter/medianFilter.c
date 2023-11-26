#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "preProcessing/SDL_Function/sdlFunction.h"

#define KERNEL_SIZE 3

/***************************************************************
 *  Function swap :
 *
 *  A simple swap function to swap a and b value  
 *
 *  @input :
 *      - a (int*) : value of a
 *      - b (int*) : value of b
***************************************************************/

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


/***************************************************************
 *  Function partition :
 *
 *  Sub function of quick sort
 *
 *  @input :
 *      - tab (int[]) : table a value
 *      - min (int) : min value of partition
 *      - max (int) : max value of the partition
 *
 *  @output :
 *      - (int) : index of the pivot
***************************************************************/

int partition(int tab[], int min, int max) {
    int pivot = tab[max];
    int i = (min - 1);

    for (int j = min; j <= max - 1; j++) {
        if (tab[j] < pivot) {
            i++;
            swap(&tab[i], &tab[j]);
        }
    }

    swap(&tab[i + 1], &tab[max]);
    return (i + 1);
}


/***************************************************************
 *  Function quickSort :
 *
 *  A simple quick sort implementation
 *
 *  @input :
 *      - tab (int[]) : table a value
 *      - min (int) : min value of partition
 *      - max (int) : max value of the partition
***************************************************************/

void quickSort(int tab[], int min, int max) {
    if (min < max) {
        int pivotI = partition(tab, min, max);
        quickSort(tab, min, pivotI - 1);
        quickSort(tab, pivotI + 1, max);
    }
}


/***************************************************************
 *  Function applyMedianFilter :
 *
 *  Apply the median filter to the input image
 *
 *  @input :
 *      - img (SDL_Surface*) : input image
 *
 *  @output :
 *      - (SDL_Surface*) : output image
***************************************************************/

SDL_Surface* applyMedianFilter(SDL_Surface* img){
    int size = KERNEL_SIZE * KERNEL_SIZE;
    int tab[KERNEL_SIZE * KERNEL_SIZE] = {-1};
    int k = KERNEL_SIZE / 2;
    SDL_Surface *outImg = SDL_ConvertSurfaceFormat
                            (
                                img,
                                SDL_PIXELFORMAT_ABGR8888,
                                0
                            );

    Uint32* inPixels = (Uint32*) img->pixels;
    Uint32* outPixels = (Uint32*) outImg->pixels;


    for (int y = 0; y < img->h; y++)
    {
        for (int x = 0; x < img->w; x++)
        {
            //complete the tab
            for (int j = y - k; j <= y + k; j++){
                for (int i = x - k; i <= x + k; i++){
                    //check valid coordonate
                    if(j >= 0 && j <= img->h &&
                        i >= 0 && i <= img->w)
                    {
                        tab[(j - (y - k)) * KERNEL_SIZE + (i - (x - k))] =
                            getPixelGrayScale(inPixels[j * img->w + i]);
                    }
                }
            }

            //Mirror padding
            for (int i = 0; i < size; i++)
            {
                if(tab[i] == -1){
                    tab[i] = size - 1 - i;
                }
            }

            //quick sort tab
            quickSort(tab, 0, size - 1);

            //get median pixel value
            int medianPxVal = tab[size/2];
            outPixels[y * img->w + x] = SDL_MapRGBA
                (
                    img->format,
                    medianPxVal,
                    medianPxVal,
                    medianPxVal,
                    255
                );
        }
    }

    return outImg;
}