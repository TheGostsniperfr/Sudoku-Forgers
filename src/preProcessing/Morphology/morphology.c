#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

//motif is a square of 3x3 of 1 value
#define MOTIF_R 2

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

                //mode 1 -> eraser
                //if pixel on motif is equal to 0
                //-> set white current px
                if(mode == 0 &&
                    getPixelGrayScale(pixels[tempY * w + tempX]) < 128)
                {

                    return 0;
                }

                //mode 0 -> dilatation
                //if pixel on motif is equal to 255
                //->set black erase current px
                else if(mode == 1 &&
                        getPixelGrayScale(pixels[tempY * w + tempX]) > 128)
                {
                    return 0;
                }
            }
        }

    }

    return 1;
}

//mode 1 -> eraser = turn off pixel
//mode 0 -> dilatation = turn on pixel


SDL_Surface* applyMorphology(SDL_Surface* img, int _mode){
    SDL_Surface* outImg = SDL_ConvertSurfaceFormat
                            (
                                img,
                                SDL_PIXELFORMAT_ABGR8888,
                                0
                            );

    Uint32* currentPixels = outImg->pixels;
    Uint32* oldPixels = img->pixels;

    mode = _mode;

    Uint32 pxMode0 = SDL_MapRGBA(img->format, 0, 0, 0, 0);
    Uint32 pxMode1 = SDL_MapRGBA(img->format, 255, 255, 255, 0);

    //get each pixels

    if(mode == 0){
        for (int y = 0; y < img->h; y++){
            for (int x = 0; x < img->w; x++)
            {
                if(oldPixels[y * img->w + x] == pxMode0){
                    continue;
                }

                if(isPixelValidMotif(oldPixels, x, y, img->w, img->h) == 0){
                    currentPixels[y * img->w + x] = pxMode0;
                }
            }
        }
    }

    if(mode == 1){
        for (int y = 0; y < img->h; y++){
            for (int x = 0; x < img->w; x++)
            {
                if(oldPixels[y * img->w + x] == pxMode1){
                    continue;
                }

                if(isPixelValidMotif(oldPixels, x, y, img->w, img->h) == 0){
                    currentPixels[y * img->w + x] = pxMode1;
                }
            }
        }
    }


    return outImg;
}