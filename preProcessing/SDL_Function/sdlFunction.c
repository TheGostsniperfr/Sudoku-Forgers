#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>


SDL_Surface* loadImg(char *path){    
    SDL_Surface* image = IMG_Load(path);

    if(!image)
    {
        errx(1, "loadImg: Error to load img");        
    }

    //Convert to rgba 8888

    return image;
}

void saveImg(SDL_Surface* image, char *filename){
    IMG_SaveJPG(image, filename, 100);    
}


SDL_Surface* fillRect(SDL_Surface* img, int _x1, int _x2, int _y1, int _y2, Uint32 pixel){

    Uint32* pixels = (Uint32*) img->pixels;

    for (int x = _x1; x < _x2; x++)
    {
        for (int y = _y1; y < _y2; y++)
        {
            //set new pixel
            pixels[y * img->w + x] = pixel;
        }
    }

    return img;
}