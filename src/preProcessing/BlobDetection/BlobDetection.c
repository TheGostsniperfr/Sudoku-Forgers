#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../../../include/preProcessing/SDL_Function/sdlFunction.h"
#include <err.h>

typedef struct Point{
	int x;
	int y;
	int size;
}Point;

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

int Fill(SDL_Surface *src, Uint32* pixels, int x, int y, Uint8 R, 
Uint8 G, Uint8 B, char** blob) {
	//Variables for color comparison and result
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint32 rgb2 = R + G + B;
	Uint32 rgb = r + g + b;
	int res = 1;

	blob[x][y] = 1;
	//Iterates through the blob's neighbours to find neighbours and colors them
	//in blue
	if (((x>=0) && (x<src->w))&&((y>=0)&&(y<src->h)))
	{
		SDL_GetRGB(pixels[y*src->w +  x], src->format, &r, &g, &b);
		rgb = r + g + b;
		if (rgb == rgb2){
			res = res +Fill(src, pixels, x, y+1, R, G, B, blob);
			res = res +Fill(src, pixels, x, y-1, R, G, B, blob);
			res = res +Fill(src, pixels, x+1, y, R, G, B, blob);
			res = res +Fill(src, pixels, x-1, y, R, G, B, blob);
			res = res +Fill(src, pixels, x+1, y+1, R, G, B, blob);
			res = res +Fill(src, pixels, x-1, y+1, R, G, B, blob);
			res = res +Fill(src, pixels, x-1, y-1, R, G, B, blob);
			res = res +Fill(src, pixels, x+1, y-1, R, G, B, blob);
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
 * 		- size_max (int*) : size of the max blob
***************************************************************/

SDL_Surface* Blob(SDL_Surface* src, int* size_max){
	//creates pixel array from the image
	Uint32* pixels = src->pixels;

	char** blob;
	int len = 0;
	Point max;


	for(int i = 0; i<src->h; i++){
		for(int j = 0;j<src->w;j++){
			blob[j][i] = 0;
		}
	}

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
			if (rgb==0){
				size = Fill(src, pixels, j, i, r, g, b, blob);
				Point temp = {j, i, size};
				if (size>area_max){
					area_max = size;
					max = temp;
					printf("%d\n", area_max);
				}
			}
			blob[j][i] = 1;
		}
	}
	src = SDL_CreateRGBSurface(0, src->w, src->h, src->format->BitsPerPixel,
     src->format->Rmask, src->format->Gmask, src->format->Bmask, src->format->Amask);
	pixels = src->pixels;
	//Cleans smallest blobs
	for (int i = 0; i<src->h;i++){
		for(int j = 0; j<src->w; j++){
			if(blob[j][i]){
				
			}
		}
	}
	printf("ok pour small blob\n");
	src = SDL_CreateRGBSurfaceFrom(pixels,
	src->w,
	src->h,
	src->format->BitsPerPixel,
	src->pitch,
	src->format->Rmask,
	src->format->Gmask,
	src->format->Bmask,
	src->format->Amask);
	IMG_SaveJPG(src, "BlobResult.jpg", 100);
	
	//modifies pointer size and returns the surface
	*size_max = area_max;
	return src;
}

int main (int argc, char* argv[]){
	if (argc!= 2){
		return 1;
	}
	SDL_Surface* src = IMG_Load(argv[1]);
	if (src == NULL){
		return 1;
	}
	src = SDL_ConvertSurfaceFormat(src, SDL_PIXELFORMAT_ABGR8888, 0);
	int size = 0;
	Blob(src, &size);
	printf("%d\n", size);
}