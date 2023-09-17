#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>


SDL_Surface* loadImg(char *path){    
    SDL_Surface* image = IMG_Load(path);

    if(!image)
    {
        errx(1, "loadImg: Error to load img");        
    }

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


/*

int main(int argc, char *argv[]) {

    if(argc != 2){
        errx(1, "Usage : %s : <image_path>", argv[0]);
        return 1;
    }    

    saveImg(loadImg(argv[1]), "test.jpeg");

    return 0;
}*/