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

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN){
                return p[0] << 16 | p[1] << 8 | p[2];
            }
            else {
                return p[0] | p[1] << 8 | p[2] << 16;
                break;
            }

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}


SDL_Surface* fillRect(SDL_Surface* img, int _x1, int _x2, int _y1, int _y2, Uint32 pixel){

    for (int x = _x1; x < _x2; x++)
    {
        for (int y = _y1; y < _y2; y++)
        {
            //set new pixel
            ((Uint32 *)img->pixels)[y * img->w + x] = pixel;
        }
    }

    return img;
}