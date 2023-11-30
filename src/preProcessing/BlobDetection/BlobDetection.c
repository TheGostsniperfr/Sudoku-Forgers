#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../../../include/preProcessing/BlobDetection/BlobDetection.h"
#include <err.h>
#include "../../../include/preProcessing/SDL_Function/sdlFunction.h"

int size_blob = 0;

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
void Fill(SDL_Surface *src, Uint32* pixels, int x, int y, int* blob, int lim){
	//Size of the blob and colour value of the current pixel
	int width = src->w;
	int pxVal = getPixelGrayScale(pixels[y*width +  x]);

	//Iterates through the blob's neighbours to find neighbour
	if (pxVal <255 && blob[y*width+x]<lim){
		size_blob += 1;
		blob[y*width+x]=lim;
		if (y<src->h-1){
			Fill(src, pixels, x, y+1, blob, lim);
		}
		if (y>0){
			Fill(src, pixels, x, y-1, blob, lim);
		}
		if (x<width-1){
			Fill(src, pixels, x+1, y, blob, lim);
		}
		if (x>0){
			Fill(src, pixels, x-1, y, blob, lim);
		}
	}
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
SDL_Surface* Blob(SDL_Surface* img, int* size_max){
	SDL_Surface *src = SDL_ConvertSurfaceFormat
						(
							img, 
							SDL_PIXELFORMAT_ARGB8888, 
							0
						);

	int width = src->w;
	int height = src->h;

	//creates pixel array from the image
	Uint32* pixels = src->pixels;

	//initialises the array of bool and the max point
	int* blob = calloc(width * height, sizeof(int));
	int max =  0;

	//error handling for array creation
	if (pixels==NULL||blob ==NULL){
		warn("Not enough storage for this operation");
	}

	//area_max will be the max size of a black blob while size
	//is the size of the current blob
	int max_size = 0;
	int lim = 1;

	//Basic colors
	Uint32 whitePx = SDL_MapRGB(src->format, 255, 255, 255);

	//Runs through the image to find the biggest blob
	for(int i = 0; i <height;i++){
		for(int j =0;j<width;j++){
			int pxVal = getPixelGrayScale(pixels[i*width +  j]);
			if (pxVal<255 && blob[i*width+j]==0){
				Fill(src, pixels, j, i, blob, lim);
				if (size_blob>max_size){
					max = lim;
					max_size = size_blob;
				}
				size_blob = 0;
				lim++;
			}
		}
	}

	//Runs through the image again and repaint the smaller blobs
	for(int i = 0; i < src -> h; i++){
		for(int j = 0; j < src -> w; j++){
			int pxVal = getPixelGrayScale(pixels[i*width +  j]);
			if (blob[i*width+j]!=max && pxVal<255){
				pixels[i*width+ j] = whitePx;
			}
		}
	}
	
	//Modifies pointer size, free the allocated and returns the surface
	*size_max = max_size;
	free(blob);

	return src;
}