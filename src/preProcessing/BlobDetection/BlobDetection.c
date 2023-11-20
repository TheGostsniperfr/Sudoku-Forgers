#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/BlobDetection/BlobDetection.h"
#include <err.h>
#include "preProcessing/SDL_Function/sdlFunction.h"

/*****************************************************************************
 *  Function Fill:
 *
 *  Search neighbiurs of the same colours (blobs) and counts size
 *  of it
 *
 *  @input :
 *      - src (SDL_Surface*) : Image to modify
 * 		- pixels (Uint32*) : Array of pixels from src
 * 		- x,y (int) : coordinates of the pixel to start iterating from
 * 		- blob (char*) : array of boolean
 * 		- lim (char) : number of iteration of the function
 *
 * 	@output :
 * 		- (int) : Size of the blob
******************************************************************************/
int Fill(SDL_Surface *src, Uint32* pixels, int x, int y, int* blob, char lim){
	//Size of the blob and colour value of the current pixel
	int res = 1;
	int pxVal = getPixelGrayScale(pixels[y*src->w +  x]);

	//Iterates through the blob's neighbours to find neighbour
	if (pxVal < 255 && *((blob+y*src->w)+x)==lim){
		*((blob+y*src->w)+x)+=1;
		if (y<src->h){
			res += Fill(src, pixels, x, y+1, blob, lim);
		}
		if (y>0){
			res += Fill(src, pixels, x, y-1, blob, lim);
		}
		if (x<src->w){
			res += Fill(src, pixels, x+1, y, blob, lim);
		}
		if (x>0){
			res += Fill(src, pixels, x-1, y, blob, lim);
		}
	}
	return res;
}

/***************************************************************
 *  Function Blob:
 *
 *  Finds grid by performing Blob Detection
 *
 *  @input :
 *      - src (SDL_Surface*) : Surface of the image to modify
 * 		- size_max (int*) : address of the size of the max blob
***************************************************************/
SDL_Surface* Blob(SDL_Surface* src, int* size_max){
	//creates pixel array from the image
	Uint32* pixels = src->pixels;

	//initialises the array of bool and the max point
	int* blob = calloc(src->w * src->h, sizeof(int));
	Point max = {0, 0, 0};

	//error handling for array creation
	if (pixels==NULL||blob ==NULL){
		warn("Not enough storage for this operation");
	}

	//area_max will be the max size of a black blob while size
	//is the size of the current blob
	int size = 0;

	//Basic colors
	Uint32 whitePx = SDL_MapRGB(src->format, 255, 255, 255);

	//Runs through the image to find the biggest blob
	for(int i = 0; i <src->h;i++){
		for(int j =0;j<src->w;j++){
			int pxVal = getPixelGrayScale(pixels[i*src->w +  j]);
			if (pxVal<255){
				size = Fill(src, pixels, j, i, blob, 0);
				Point temp = {j, i, size};
				if (size>max.size){
					max = temp;
				}
			}
		}
	}

	//Highlights the blob
	Fill(src, pixels, max.x, max.y, blob, 1);

	//Runs through the image again and repaint the smaller blobs
	for(int i = 0; i < src -> h; i++){
		for(int j = 0; j < src -> w; j++){
			int pxVal = getPixelGrayScale(pixels[i*src->w +  j]);
			if (*((blob+i*src->w)+j)<2){
				if(pxVal < 255){
					pixels[i*src->w + j] = whitePx;
				}
			}
		}
	}

	//Modifies pointer size, free the allocated and returns the surface
	*size_max = max.size;
	free(blob);
	return src;
}