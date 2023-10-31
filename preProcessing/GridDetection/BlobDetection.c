#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "../Rotation/rotation.h"
#include <err.h>

/***************************************************************
 *  Function Blob: 
 *
 *  Finds grid by performing Blob Detection
 *
 *  @input :
 *      - (char*) : Path towards the image
***************************************************************/

void Blob(char *path){
	//loads image from path
	SDL_Surface *src = loadImg(path);

	//creates pixel array from the image
	Uint32* pixels = src->pixels;

	//error if array creation returns an error
	if (pixels==NULL){
		warn("Not enough storage for this operation");
	}

	//area_max will be the max size of a black segment while size
	//is the size of the current black segment
	int area_max = 0;
	int size = 0;

	//rgb variables for color comparison
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 rgb = 0;

	//runs through the image to find the biggest segments
	//(horizontally)
	for(int i = 0; i<src->h;i++){
		for(int j =0;j<src->w;j++){
			SDL_GetRGB(pixels[i*src->h +  j], src->format, &r, &g, &b);
			rgb = (r+g+b)/3;
			if (rgb==0){
				size++;
			}
			if (rgb==255 && size!=0){
				if (size>area_max){
					area_max = size;
				}
				size = 0;
			}
		}

		//higlights biggest segments and rotates image
		Highlight(&dest, &pixels, area_max);
		dest = Rotated_image(&dest,90);

		//runs through the image to find the biggest segments again 
		//(vertically)
		for(int i = 0; i<src->h;i++){
			for(int j =0;j<src->w;j++){
				SDL_GetRGB(pixels[i*src->h +  j], src->format, &r, &g, &b);
				rgb = (r+g+b)/3;
				if (rgb==0){
					size++;
				}
				if (rgb==255 && size!=0){
					if (size>area_max){
						area_max = size;
					}
						size = 0;
			}
		}

		//Highlights and rotates the image back to its original angle
		Highlight(&dest, &pixels, area_max);
		dest = Rotated_image(&dest,90);
	}
}

/***************************************************************
 *  Function Clean: 
 *
 *  Only keeps biggest blobs : the grid
 *
 *  @input :
 *      - src (SDL_Surface*) : Image to clean
 * 		- pixels (Uint32*) : Array of pixels from src
 * 		- size (int) : Size of the biggest segments
***************************************************************/

void Clean(SDL_Surface *src, Uint32 *pixels, int size) {
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 rgb = 0;
	for (int i = 0; i<src->h;i++){
		for (int j = 0; j<src->w,j++){

		}
	}
}