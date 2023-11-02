#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
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
	SDL_Surface* src = loadImg(path);

	//creates pixel array from the image
	Uint32* pixels = src->pixels;

	//error handling for array creation and surface creation from path
	if ((pixels==NULL)||(src==NULL)){
		warn("Not enough storage for this operation");
	}

	//area_max will be the max size of a black blob while size
	//is the size of the current blob
	int area_max = 0;
	int size = 0;
	int max_x = NULL;
	int max_y = NULL;

	//rgb variables for color comparison
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 rgb = 0;

	//runs through the image to find the biggest blob
	for(int i = 0; i<src->h;i++){
		for(int j =0;j<src->w;j++){
			SDL_GetRGB(pixels[i*src->h +  j], src->format, &r, &g, &b);
			rgb = (r+g+b)/3;
			if (rgb==255){
				size = Fill(src, pixels, j, i, 0, 0, 255, 0);
				if (size>area_max){
					if (max_x != NULL){
						Fill(src, pixels, max_x,max_y,255,255,255, 0);
					}
					area_max = size;
					max_x = j;
					max_y = i;
				}
			}
		}
	}

	//Cleans smallest blobs
	for (int i = 0; i<src->h;i++){
		for (int j = 0; j<src->w; j++){
			SDL_GetRGB(pixels[i*src->h +  j], src->format, &r, &g, &b);
			rgb = (r+g+b)/3;
			if (rgb==255/3){
				size = Fill(src, pixels, j, i, 0, 0, 0, 1);
				if (size<area_max){
					Fill(src, pixels, i, j, 255, 255, 255, 0);
				}
			}
		}
	}

	//creates a surface from the pixel array and saves it
	SDL_Surface* res = SDL_CreateRGBSurfaceFrom(*pixels,
	src->w, 
	src->h,
	src->format->BitsPerPixel,
	src->pitch,
	src->format->Rmask,
	src->format->Gmask,
	src->format->Bmask,
	src->format->Amask);
	
	saveImg(res, "BlobResult.jpg"); 
	}



/***************************************************************
 *  Function Fill: 
 *
 *  Fills all pixels connected by the same color and counts size
 *  of the current blob
 *
 *  @input :
 *      - src (SDL_Surface*) : Image to modify
 * 		- pixels (Uint32*) : Array of pixels from src
 * 		- x,y (int) : coordinates of the pixel to start filling from
 * 		- R,G,B (Uint8) : color in which the pixels have to change
 * 		- Func (char) : if 0 : counts size and colors, if 1 : only counts
***************************************************************/

int Fill(SDL_Surface *src, Uint32* pixels, int x, int y, Uint8 R, Uint8 G, Uint8 B, char Func) {
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 rgb = 0;
	int res = 1;
	//
	for (int i = 0; i<src->h;i++){
		for (int j = 0; j<src->w;j++){
			if (Func == 1){
				pixels[y*src->h+x] = new_pixel(R, G, B);
			}
			if (y +1<src->h){
				SDL_GetRGB(pixels[(y+1)*src->h +  x], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x, y+1, R, G, B, Func);
				}
			}
			if (y -1>0){
				SDL_GetRGB(pixels[(y-1)*src->h +  x], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x, y-1, R, G, B, Func);
				}
			}
			if (x +1<src->w){
				SDL_GetRGB(pixels[y*src->h +  x+1], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x+1, y, R, G, B, Func);
				}
			}
			if (x-1>0){
				SDL_GetRGB(pixels[y*src->h + x-1], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x-1, y, R, G, B, Func);
				}
			}
			if (y +1<src->h){
				SDL_GetRGB(pixels[i*src->h +  j], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x, y+1, R, G, B, Func);
				}
			}
			if (y +1<src->h){
				SDL_GetRGB(pixels[i*src->h +  j], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x, y+1, R, G, B, Func);
				}
			}
			if (y +1<src->h){
				SDL_GetRGB(pixels[i*src->h +  j], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x, y+1, R, G, B, Func);
				}
			}
			if (y +1<src->h){
				SDL_GetRGB(pixels[i*src->h +  j], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x, y+1, R, G, B, Func);
				}
			}
		}
	}

	return res;
}