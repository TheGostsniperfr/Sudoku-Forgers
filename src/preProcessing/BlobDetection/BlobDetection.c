#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include <err.h>

#include "preProcessing/BlobDetection/BlobDetection.h"

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
int Fill(SDL_Surface *src, Uint32* pixels, int x, int y, int x2, int y2,
	Uint8 R, Uint8 G, Uint8 B, char** blob, char Func) {
	//Variables for color comparison and result
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint32 rgb2 = R + G + B;
	Uint32 rgb = r + g + b;
	int res = 1;

	x2 = 1;
	if(Func==1){
		pixels[y* src->w +x] = SDL_MapRGB(src->format, 0, 0, 0);
	}
	//Iterates through the blob's neighbours to find neighbours and colors them
	//in blue
	if (((x>=0) && (x<src->w))&&((y>=0)&&(y<src->h)))
	{
		SDL_GetRGB(pixels[y*src->w +  x], src->format, &r, &g, &b);
		rgb = r + g + b;
		if (rgb == rgb2){

			res = res +Fill(src, pixels, x, y+1, x2, y2+1,R, G, B, blob, Func);
			res = res +Fill(src, pixels, x, y-1, x2, y2-1,R, G, B, blob, Func);
			res = res +Fill(src, pixels, x+1, y, x2+1, y2, R, G, B, blob, Func);
			res = res +Fill(src, pixels, x-1, y, x2-1, y2, R, G, B, blob, Func);
			res = res +Fill(src, pixels, x+1, y+1, x2+1, y2+1,R, G, B, blob, Func);
			res = res +Fill(src, pixels, x-1, y+1, x2-1, y2+1, R, G, B, blob, Func);
			res = res +Fill(src, pixels, x-1, y-1, x2-1, y2-1, R, G, B, blob, Func);
			res = res +Fill(src, pixels, x+1, y-1, x2+1, y2-1, R, G, B, blob, Func);
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

	//char** blob = malloc(sizeof(char*)*src->h);
	Point max;
	//Remove, it's for avoid warn
	max.x=0;
	max.y=0;
	max.size = 0;


	for(int i = 0; i < src->h; i++){
		//i = malloc(sizeof(char)*src->w);
		for(int *j = i;j<src->w;j++){
			//*j = 0;
			if (i == j) {}
		}
	}

	//error handling for array creation
	if (pixels==NULL){
		warn("Not enough storage for this operation");
	}

	//area_max will be the max size of a black blob while size
	//is the size of the current blob
	int area_max = 0;
	//int size = 0;

	//rgb variables for color comparison
	//Uint8 r = 0;
	//Uint8 g = 0;
	//Uint8 b = 0;
	//Uint32 rgb = 0;
	//int y = 0;
	//int* x = 0;

	//runs through the image to find the biggest blob
	/*for(int i = 0; i<src->h;i++){
		if(y ==0){
			y = blob;
		}
		else{
			y+=1;
		}
		for(int j =0;j<src->w;j++){
			if(x==0){
				x = y;
			}
			else{
				x++;
			}
			SDL_GetRGB(pixels[i*src->w +  j], src->format, &r, &g, &b);
			rgb = (r+g+b)/3;
			if (rgb==0){
				size = Fill(src, pixels, j, i, x, y, r, g, b, blob, 0);
				Point temp = {j, i, size};
				if (size>area_max){
					area_max = size;
					max = temp;
					printf("%d\n", area_max);
				}
			}
			blob[j][i] = 1;
		}
	}*/
	src = SDL_CreateRGBSurface(0, src->w, src->h, src->format->BitsPerPixel,
     src->format->Rmask, src->format->Gmask, src->format->Bmask, src->format->Amask);
	pixels = src->pixels;
	for (int i = 0; i<src->h;i++){
		for(int j = 0; j<src->w; j++){
			pixels[i* src->w +j] = SDL_MapRGB(src->format, 0, 0, 0);
		}
	}
	//Cleans smallest blobs
	for (int i = 0; i<src->h;i++){
		for(int j = 0; j<src->w; j++){
			if((j==max.x)&&(i==max.y)){
				//Fill(src, pixels, j, i, x, y, 255,  255, 255, blob, 1);
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

	//Modifies pointer size and returns the surface
	*size_max = area_max;
	return src;
}