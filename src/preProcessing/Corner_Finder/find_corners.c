#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "math.h"

/***************************************************************
 *  Function detecterCoins:
 *
 *  Fonction to find all the corner of the sudoku's grid
 *
 *  @input :
 *      -
 *  @output :
 *      -
***************************************************************/
int* FindCoins(SDL_Surface* surface) {
    int* points = (int*)malloc(8 * sizeof(int));
    // Initialisation of all the max/min values
    int minSum = surface->w + surface->h;
    int maxSum = 0;
    int minDiff = surface->w > surface->h ? surface->w : surface->h;
    int maxDiff = -1 * minDiff;

    Uint32* pixels = (Uint32*)surface->pixels;

    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint8 pixelValue = pixels[y * surface->w + x];
            if (pixelValue != 0xff) {
                // If the pixel is white continue without entering the "if"
                // Else we verify if we are at a corner

                int sum = x + y;
                int diff = x - y;

                if (sum < minSum) {
                    minSum = sum;
                    points[0] = x; // upper left corner -> x
                    points[1] = y; // upper left corner -> y
                }

                if (sum >= maxSum) {
                    maxSum = sum;
                    points[4] = x; // lower right corner -> x
                    points[5] = y; // lower right corner -> y
                }

                if (diff < minDiff) {
                    minDiff = diff;
                    points[2] = x; // lower left corner -> x
                    points[3] = y; // lower left corner -> y
                }

                if (diff > maxDiff) {
                    maxDiff = diff;
                    points[6] = x; // upper right corner -> x
                    points[7] = y; // upper right corner -> y
                }
            }
        }
    }
    return points;
}