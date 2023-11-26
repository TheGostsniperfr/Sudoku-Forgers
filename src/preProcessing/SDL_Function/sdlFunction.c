#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>


/***************************************************************
 *  Function loadImg:
 *
 *  Function to load and convert image in SDL_PIXELFORMAT_ABGR8888
 *  pixel format
 *
 *  @input :
 *      - path (*path) : path of the image to load
 *  @output :
 *      - output image (SDL_Surface*) : the image loaded
***************************************************************/

SDL_Surface* loadImg(char *path){
    SDL_Surface* image = IMG_Load(path);

    if(!image)
    {
        errx(1, "loadImg: Error to load img");
    }

    //Convert to rgba 8888
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ABGR8888, 0);


    return image;
}


/***************************************************************
 *  Function saveImg:
 *
 *  Function to save an image
 *
 *  @input :
 *      - image (SDL_Surface*) : the image to save
 *      - filename (char *) : the name of the image to save
***************************************************************/

void saveImg(SDL_Surface* image, char *filename){
    IMG_SaveJPG(image, filename, 100);
}


/***************************************************************
 *  Function drawRect :
 *
 *  A simple funtion to draw easier a sdl rectangle
 *
 *  @input :
 *      - surface (SDL_Surface*) : input image
 *      - x0 (int) : x coordonate of upper left corner
 *      - y0 (int) : y coordonate of upper left corner
 *      - w (int) : width of the rectangle to draw
 *      - h (int) : height of the rectangle to draw
 *      - pixel (Uint32) : pixel to fill 
***************************************************************/

void drawRect(SDL_Surface* surface, int x0,
                int y0, int w, int h, Uint32 pixel)
{
    SDL_Rect rect =  {x0, y0, w ,h};
    SDL_FillRect(surface, &rect, pixel);
}


/***************************************************************
 *  Function getPixelGrayScale :
 *
 *  A simple function to get the grayscale value of a pixel
 *  Based on the blue pixel
 *
 *  @input :
 *      - pixel (Uint32) : pixel to get grayscale value
 *
 *  @output :
 *      - (int) : grayscale value of the pixel
***************************************************************/

int getPixelGrayScale(Uint32 pixel){
    return (pixel>>16) & 0xFF;
}