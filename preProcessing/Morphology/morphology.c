#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

//motif is a square of 3x3 of 1 value
#define MOTIF_R 1

static int mode;

//return 1 if the pixel coordonate is valid, else 0
int isPixelValid(int x, int y, int w, int h){
    if(x>= 0 && x <= w){
        if(y>= 0 && y <= h){
            return 1;
        }
    }
    return 0;
}

//return 1 if the motif is respected for the current pixel
int isPixelValidMotif(Uint32* pixels, int x, int y, int w, int h){
    //check pixel with motif
    for (int tempX = x-MOTIF_R; tempX <= x + MOTIF_R; tempX++)
    {
        for (int tempY = y-MOTIF_R; tempY <= y+MOTIF_R ; tempY++)
        {        
            


            if(isPixelValid(tempX, tempY, w, h) == 1){
                if(getPixelGrayScale(pixels[tempY * w + tempX]) < 128 * mode){
                    return 0;
                }
            }
        }
        
    }

    return 1;
}

Uint32 getPixelByMode(SDL_PixelFormat* format){
    if(mode == 0){
        //black
        return SDL_MapRGBA(format, 0, 0, 0, 0);
    }

    //white
    return SDL_MapRGBA(format, 255, 255, 255, 0);
}

//mode 1 -> eraser = turn off pixel
//mode 0 -> dilatation = turn on pixel


SDL_Surface* applyMorphology(SDL_Surface* img, int _mode){
    SDL_Surface* outImg = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_ABGR8888,0);
    Uint32* currentPixels = outImg->pixels;
    Uint32* oldPixels = img->pixels;

    mode = _mode;
    

    //get each pixels
    for (int y = 0; y < img->h; y++)
    {
        for (int x = 0; x < img->w; x++)
        {
            //check if we need to check the pixel
            if(oldPixels[y * img->w + x] == getPixelByMode(img->format)){
                continue;
            }

            //check if we need to switch de pixel
            if(isPixelValidMotif(oldPixels, x, y, img->w, img->h) == 0){
                currentPixels[y * img->w + x] = getPixelByMode(img->format); 
            }            
        }        
    }

    return outImg;    
}