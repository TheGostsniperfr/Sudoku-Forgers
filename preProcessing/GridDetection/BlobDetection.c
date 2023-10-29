#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "../Pixel/pixel.h"
#include <err.h>

/***************************************************************
 *  Function Blob: 
 *
 *  Finds grid by performing Blob Detection
 *
 *  @input :
 *      - (char*) : path towards the image
 * 	@output :
 * 		- (SDL_Surface*) : The image modified to highlight grid
***************************************************************/

SDL_Surface* Blob(char *path){
	SDL_Surface *src = loadImg(path);
	SDL_Surface *dest = SDL_CreateRGBSurface(0, src->w, src->h, 
		src->format);
	if (dest==NULL){
		warn("Not enough storage for this operation");
	}
	int area_max = 0;
	int size = 0;
	for(int i = 0; i<src->h;i++){
		for(int j =0;j<src->w;j++){
			Point temp = {j, i};
			if (getPixel8(*dest, j, i)==255){
				size++;
			}
			if (getPixel8(*dest, j, i)==0 && size!=0){
				if (size>area_max){
					
				}
				size = 0;
			}
		}
	}
}