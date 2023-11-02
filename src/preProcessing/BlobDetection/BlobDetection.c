#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../../../preProcessing/SDL_Function/sdlFunction.h"
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
 * 		- R,G,B (Uint8) : 
 * 		- R2, G2, B2 (Uint8) : color in which the pixels have to change
 * 		- Func (char) : if 0 : counts size and colors, if 1 : only counts size
 * 
 * 	@output :
 * 		- (int) : Size of the blob
******************************************************************************/

int Fill(SDL_Surface *src, Uint32* pixels, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 R2, Uint8 G2, Uint8 B2) {

	//Variables for color comparison and result
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	int res = 1;

	//Iterates through the blob to find neighbours and colors them if Func = 1
	for (int i = 0; i<src->h;i++){
		for (int j = 0; j<src->w;j++){
			pixels[y*src->h+x] = SDL_MapRGB(src->format,R2, G2, B2);
			if (y +1<src->h){
				SDL_GetRGB(pixels[(y+1)*src->h +  x], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x, y+1, R, G, B, R2, G2, B2);
				}
			}
			if (y -1>0){
				SDL_GetRGB(pixels[(y-1)*src->h +  x], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x, y-1, R, G, B, R2, G2, B2);
				}
			}
			if (x +1<src->w){
				SDL_GetRGB(pixels[y*src->h +  x+1], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x+1, y, R, G, B, R2, G2, B2);
				}
			}
			if (x-1>0){
				SDL_GetRGB(pixels[y*src->h + x-1], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x-1, y, R, G, B, R2, G2, B2);
				}
			}
			if (y +1<src->h && x+1<src->w){
				SDL_GetRGB(pixels[(y+1)*src->h +  (x+1)], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x+1, y+1, R, G, B, R2, G2, B2);
				}
			}
			if (y +1<src->h && x-1>0){
				SDL_GetRGB(pixels[(y+1)*src->h +  (x-1)], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x-1, y+1, R, G, B, R2, G2, B2);
				}
			}
			if (y -1<src->h && x-1>0){
				SDL_GetRGB(pixels[(y-1)*src->h +  (x-1)], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x-1, y-1, R, G, B, R2, G2, B2);
				}
			}
			if (y-1<src->h && x+1<src->w){
				SDL_GetRGB(pixels[(y-1)*src->h +  (x+1)], src->format, &r, &g, &b);
				if(r == R && g == G && b == B){
					res = res +Fill(src, pixels, x+1, y-1, R, G, B, R2, G2, B2);
				}
			}
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
 *      - (src*) : Surface of the image to modify
***************************************************************/

void Blob(SDL_Surface* src){
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
	int max_x = -1;
	int max_y = -1;

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
				Fill(src, pixels, j, i, r, g, b, 0, 0, 255);
				if (size>area_max){
					if (max_x >= 0){
						Fill(src, pixels, max_x,max_y,r, g, b,255,255,255);
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
				size = Fill(src, pixels, j, i, r, g, b, 0, 0, 0);
				if (size<area_max){
					Fill(src, pixels, i, j, r, g, b, 255, 255, 255);
				}
			}
		}
	}

	//creates a surface from the pixel array and saves it
	src = SDL_CreateRGBSurfaceFrom(pixels,
	src->w, 
	src->h,
	src->format->BitsPerPixel,
	src->pitch,
	src->format->Rmask,
	src->format->Gmask,
	src->format->Bmask,
	src->format->Amask);
	
	}

	int main(int argc, char* argv[]) {
    if (argc!=2){
        return 1;
    }
	SDL_Surface* image = IMG_Load(argv[1]);
	if (image == NULL)
	{
		warn("Not enough storage for this operation");
		return 1;
	}
	image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ABGR8888, 0);

    Blob(image);
	}
