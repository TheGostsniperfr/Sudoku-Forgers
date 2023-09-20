#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#include <stdio.h>


/***************************************************************
 *  Function findOtsuThreshold: 
 *
 *  Find the optimal threshold using Otsu method
 *
 *  @input :
 *      - histogram (double*) : histogram of the image
 *      - numPixels (int) : total number of pixel in the image
 *  @output :
 *      - threshold (double) : the optimal threshold
***************************************************************/
double findOtsuThreshold(double* histogram, int numPixels) {

    double sigma_max = -1;
    int optimal_threshold = 0;

    for (int t = 0; t < 256; t++) {
        double w0 = 0;
        for (int i = 0; i <= t; i++) {
            w0 += histogram[i];
        }

        double w1 = 1.0 - w0;

        double sum0 = 0;
        for (int i = 0; i <= t; i++) {
            sum0 += (histogram[i] / numPixels) * i;
        }

        double sum1 = 0;
        for (int i = t + 1; i < 256; i++) {
            sum1 += (histogram[i] / numPixels) * i;
        }

        if (w0 < 1e-10 || w1 < 1e-10) {
            continue;
        }

        double u0 = sum0 / w0;
        double u1 = sum1 / w1;

        double diff = u0 - u1;

        double sigma = w0 * w1 * diff * diff;

        if (sigma_max < sigma) {
            sigma_max = sigma;
            optimal_threshold = t; 
        }
    }

    return optimal_threshold;
}


/***************************************************************
 *  Function binarization: 
 *
 *  Binarization of the image, set p(i) white if p(i).Value > threshold, else p(i) = black
 *
 *  @input :
 *      - img (SDL_Surface*) : input image to binarisied
 *      - threshold (double) : the optimal threshold of the input image
 *  @output :
 *      - output image (SDL_Surface*) : the binarised image (with otsu method)
***************************************************************/
SDL_Surface* binarization(SDL_Surface* img, double threshold){
    Uint32* pixels = img->pixels;

    SDL_Surface *outputSurface = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_ABGR8888, 0);

    Uint32* pixelsOut = outputSurface->pixels;


    for (int x = 0; x < img->w; x++)
    {
        for (int y = 0; y < img->h; y++)
        {
            //get pixel
            Uint32 pixel = pixels[y * img->w + x];

            //get color of the pixel
            Uint8 r, ignore_g, ignore_b, ignore_a;
            SDL_GetRGBA(pixel, img->format, &r, &ignore_g, &ignore_b, &ignore_a);

            if(r > threshold){
                pixelsOut[y * img->w + x] = SDL_MapRGBA(img->format, 255, 255, 255, 255);
            }else{
                pixelsOut[y * img->w + x] = SDL_MapRGBA(img->format, 0, 0, 0, 255);
            }
        }        
    }

    return outputSurface;
}