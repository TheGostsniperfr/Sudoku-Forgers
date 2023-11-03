#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include <err.h>

/*****************************************************************************
 *  Function Fill:
 *
 *  Fills all pixels connected by the same color and counts size
 *  of the current blob
 *
 *  @input :
 *      - src (SDL_Surface*) : Image to modify
 * 		- pixels (Uint32*) : Array of pixels from src
 * 		- x,y (int) : coordinates of the pixel to start filling from
 * 		- R,G,B (Uint8) : color of the starting pixel
 * 		- R2, G2, B2 (Uint8) : color in which the pixels have to change
 *
 * 	@output :
 * 		- (int) : Size of the blob
******************************************************************************/

int Fill(SDL_Surface *src, Uint32* pixels, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 R2, Uint8 G2, Uint8 B2) {
	//Variables for color comparison and result
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 rgb2 = R + G + B;
	int res = 1;

	//tests if pixel is the right color
	SDL_GetRGB(pixels[y*src->w +  x], src->format, &r, &g, &b);
	Uint32 rgb = r + g + b;
	if (rgb != rgb2){
		return 0;
	}

	//Iterates through the blob's neighbours to find neighbours and colors them in blue
	pixels[y*src->w+x] = SDL_MapRGB(src->format,R2, G2, B2);
	SDL_GetRGB(pixels[y*src->w +  x], src->format, &r, &g, &b);
	if (y +1<src->h){
		res = res +Fill(src, pixels, x, y+1, R, G, B, R2, G2, B2);
	}
	else if (y-1>=0){
		res = res +Fill(src, pixels, x, y-1, R, G, B, R2, G2, B2);
	}
	else if (x +1<src->w){
		res = res +Fill(src, pixels, x+1, y, R, G, B, R2, G2, B2);
	}
	else if ((x-1)>=0){
		res = res +Fill(src, pixels, x-1, y, R, G, B, R2, G2, B2);
	}
	else if ((y +1<src->h) && (x+1<src->w)){
		res = res +Fill(src, pixels, x+1, y+1, R, G, B, R2, G2, B2);
	}
	else if ((y +1<src->h) && (x-1>=0)){
		res = res +Fill(src, pixels, x-1, y+1, R, G, B, R2, G2, B2);
	}
	else if ((y-1>=0) && (x-1>=0)){
		res = res +Fill(src, pixels, x-1, y-1, R, G, B, R2, G2, B2);
	}
	else if ((y-1>=0) && (x+1<src->w)){
		res = res +Fill(src, pixels, x+1, y-1, R, G, B, R2, G2, B2);
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
 * 		- size_max (int*) : size of the max blob
***************************************************************/

SDL_Surface* Blob(SDL_Surface* src, int* size_max){
	//creates pixel array from the image
	Uint32* pixels = src->pixels;

	//error handling for array creation 
	if (pixels==NULL){
		warn("Not enough storage for this operation");
	}

	//area_max will be the max size of a black blob while size
	//is the size of the current blob
	int area_max = 0;
	int size = 0;

	//rgb variables for color comparison
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint32 rgb = 0;

	//runs through the image to find the biggest blob
	for(int i = 0; i<src->h;i++){
		for(int j =0;j<src->w;j++){
			SDL_GetRGB(pixels[i*src->w +  j], src->format, &r, &g, &b);
			rgb = (r+g+b)/3;
			if (rgb<45){
				size = Fill(src, pixels, j, i, r, g, b, 0, 0, 255);
				if (size>area_max){
					area_max = size;
				}
			}
		}
	}

	//Cleans smallest blobs
	for (int i = 0; i<src->h;i++){
		for (int j = 0; j<src->w; j++){
			SDL_GetRGB(pixels[i*src->w +  j], src->format, &r, &g, &b);
			rgb = r+g+b;
			if (rgb==255){
				size = Fill(src, pixels, j, i, r, g, b, r, g, b);
				if (size<area_max){
					Fill(src, pixels, j, i, r, g, b, 255, 255, 255);
				}
			}
		}
	}
	src = SDL_CreateRGBSurfaceFrom(pixels,
	src->w,
	src->h,
	src->format->BitsPerPixel,
	src->pitch,
	src->format->Rmask,
	src->format->Gmask,
	src->format->Bmask,
	src->format->Amask);
	
	//modifies pointer size and returns the surface
	*size_max = area_max;
	return src;
}