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
    IMG_SaveJPG(image, strcat("../data/img_export/",filename), 100);
}


int main(int argc, char *argv[]) {

    if(argc != 2){
        errx(1, "Usage : %s : <image_path>", argv[0]);
        return 1;
    }    

    saveImg(loadImg(argv[1]), "test.jpeg");

    return 0;
}