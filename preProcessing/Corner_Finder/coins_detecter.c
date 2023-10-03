#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "math.h"

#include "../Pixel/pixel.h"

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
void detecterCoins(SDL_Surface* surface, int* points) {
    // Initialisation des valeurs minimales/maximales
    int minSum = surface->w + surface->h;
    int maxSum = 0;
    int minDiff = surface->w > surface->h ? surface->w : surface->h;
    int maxDiff = -1 * minDiff;

    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            Uint8 pixelValue = getPixel8(surface, x, y);
            if (pixelValue != 0xff) {
                // Si le pixel est blanc on continue sans entrer dans la boucle
                // Sinon l'on vérifie si l'on est arrivé a un coin

                int sum = x + y;
                int diff = x - y;

                if (sum < minSum) {
                    minSum = sum;
                    points[0] = x; // coin superieur gauche -> x
                    points[1] = y; // coin superieur gauche -> y
                }

                if (sum >= maxSum) {
                    maxSum = sum;
                    points[4] = x; // coin inferieur droite -> x
                    points[5] = y; // coin inferieur droite -> y
                }

                if (diff < minDiff) {
                    minDiff = diff;
                    points[2] = x; // coin inferieur gauche -> x
                    points[3] = y; // coin inferieur gauche -> y
                }

                if (diff > maxDiff) {
                    maxDiff = diff;
                    points[6] = x; // coin superieur droite -> x
                    points[7] = y; // coin superieur droite -> y
                }
            }
        }
    }
}