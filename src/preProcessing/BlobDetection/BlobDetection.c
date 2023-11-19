#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/BlobDetection/BlobDetection.h"
#include <err.h>

#include "preProcessing/SDL_Function/sdlFunction.h"

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
 * 		- blob (char*) : array of boolean
 * 		- lim (char) : number of iteration of the function
 *
 * 	@output :
 * 		- (int) : Size of the blob
******************************************************************************/
int Fill(SDL_Surface *src, Uint32* pixels, int x, int y, char* blob, char lim){
	//Variables for color comparison and result
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint32 rgb;
	int res = 1;

	//Iterates through the blob's neighbours to find neighbours and colors them
	//in blue
	SDL_GetRGB(pixels[y*src->w +  x], src->format, &r, &g, &b);
	rgb = r + g + b;
	if (rgb == 0 && *((blob+y*src->w)+x)==lim){
		*((blob+y*src->w)+x)+=1;
		if (y<src->w-1){
			res = res + Fill(src, pixels, x, y+1, blob, lim);
		}
		if (y>0){
			res = res + Fill(src, pixels, x, y-1, blob, lim);
		}
		if (x<src->h-1){
			res = res + Fill(src, pixels, x+1, y, blob, lim);
		}
		if (x>0){
			res = res + Fill(src, pixels, x-1, y, blob, lim);
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
	char* blob = calloc(src->w * src->h, sizeof(char));
	Point max = {0, 0, 0};

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
				size = Fill(src, pixels, j, i, blob, 0);
				Point temp = {j, i, size};
				if (size>area_max){
					area_max = size;
					max = temp;
				}
			}
		}
	}

	//Highlights the blob
	Fill(src, pixels, max.x, max.y, blob, 1);

	//Runs through the image again and repaint the smaller blobs
	for(int i = 0; i<src->h; i++){
		for(int j = 0; i<src->w; j++){
			if (*((blob+i*src->w)+j)<2){
				SDL_GetRGB(pixels[i*src->w +  j], src->format, &r, &g, &b);
				rgb = (r+g+b)/3;
				if(rgb!=255){
					pixels[i*src->w +  j] = SDL_MapRGB(src->format, 255, 
					255, 255);
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
	IMG_SaveJPG(src, "BlobResult.jpg", 100);

	//Modifies pointer size, free the allocated and returns the surface
	*size_max = area_max;
	free(blob);
	return src;
}

int main(int argc, char* argv[]){
	if(argc != 2){
		warn("Pas assez d'arguments");
		return 0;
	}
	SDL_Surface *src = IMG_Load(argv[1]);
	if (src==NULL){
		warn("Pas assez d'espace");
		return 0;
	}
	int size = 0;
	Blob(src, &size);
	printf("Taille max : %d\n", size);
}