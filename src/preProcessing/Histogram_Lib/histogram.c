#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

#define HISTO_W_BAR 50

#define HISTO_WIDTH 1000  // Width of the histogram image
#define HISTO_HEIGHT 800 // Height of the histogram image


/***************************************************************
 *  Function findHistogram:
 *
 *  Calculation the histogram of the input image
 *
 *  @input :
 *      - img (SDL_Surface*) : input image to calcul the histogram
 *  @output :
 *      - (double*) : histogram of the image
***************************************************************/

double* findHistogram(SDL_Surface* img){

    //init histogram
    double* histogram = (double*)malloc(256 * sizeof(double));
    for (int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }

    Uint32* pixels = (Uint32*) img->pixels;

    for (int x = 0; x < img->w; x++)
    {
        for (int y = 0; y < img->h; y++)
        {

            //get pixel
            Uint32 pixel = pixels[y * img->w + x];

            //get color of the pixel
            Uint8 r, g, b, ignore_a;
            SDL_GetRGBA(pixel, img->format, &r, &g, &b, &ignore_a);

            histogram[(r+g+b)/3]++;
        }
    }


    //normalisation of the histogram (divised by the total of
    //pixel in the image)

    for (int i = 0; i < 256; i++) {
        histogram[i] /= (img->w * img->h);
    }



    return histogram;
}


/***************************************************************
 *  Function createHistogramImg:
 *
 *  Create a graphic representation of the histogram (bar graph)
 *
 *  @input :
 *      - histogram (double*) : histogram of an image
 *  @output :
 *      - histogram image (SDL_Surface*) : the graphic representation
 *                                          of the image histogram
***************************************************************/

SDL_Surface* createHistogramImg(double* histogram) {
    double maxVal = histogram[0];

    for (int i = 0; i < 256; i++) {
        if (maxVal < histogram[i]) {
            maxVal = histogram[i];
        }
    }

    SDL_Surface* outputSurface = SDL_CreateRGBSurfaceWithFormat
                                    (
                                        0,
                                        HISTO_WIDTH,
                                        HISTO_HEIGHT,
                                        32,
                                        SDL_PIXELFORMAT_ABGR8888
                                    );

    Uint32 pixel = SDL_MapRGBA(outputSurface->format, 255, 0, 0, 0);

    for (int x = 0; x < 256; x++) {
        int barHeight = (int)((histogram[x] / maxVal) * HISTO_HEIGHT);

        SDL_Rect rect;
        rect.x = x * (HISTO_WIDTH / 256);
        rect.y = HISTO_HEIGHT - barHeight;
        rect.w = HISTO_WIDTH / 256;
        rect.h = barHeight;

        SDL_FillRect(outputSurface, &rect, pixel);
    }

    return outputSurface;
}