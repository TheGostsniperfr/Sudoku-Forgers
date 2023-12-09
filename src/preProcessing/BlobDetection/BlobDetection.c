#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/BlobDetection/BlobDetection.h"
#include <err.h>
#include "preProcessing/SDL_Function/sdlFunction.h"

#include "math.h"

int size_blob = 0;

/*****************************************************************************
 *  Function Fill:
 *
 *  Search neighbours of the same colours (blobs) and counts size
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
void Fill(SDL_Surface *src, Uint32* pixels, int x, int y, int* blob, int lim)
{
    int width = src->w;
    int height = src->h;

    // Creation of the stack
    Pointx_y* stack = malloc(width * height * sizeof(Pointx_y));
    if (stack == NULL) {
        // Alloc error handling
        fprintf(stderr, "Erreur d'allocation mémoire pour la pile.\n");
        exit(EXIT_FAILURE);
    }

    int ind = 0;

    //Stack the first pixel
    stack[ind].x = x;
    stack[ind].y = y;
    ind++;

    while (ind > 0) {
        // Depile a pixel
        ind--;
        x = stack[ind].x;
        y = stack[ind].y;

        //Size of the blob and colour value of the current pixel
        int pxVal = getPixelGrayScale(pixels[y * width + x]);

        if (pxVal < 255 && blob[y * width + x] < lim) {
            size_blob += 1;
            blob[y * width + x] = lim;

            //Stack neighbours if they match condition
            if (y < height - 1) {
                stack[ind].x = x;
                stack[ind].y = y + 1;
                ind++;
            }
            if (y > 0) {
                stack[ind].x = x;
                stack[ind].y = y - 1;
                ind++;
            }
            if (x < width - 1) {
                stack[ind].x = x + 1;
                stack[ind].y = y;
                ind++;
            }
            if (x > 0) {
                stack[ind].x = x - 1;
                stack[ind].y = y;
                ind++;
            }
        }
    }

    //Free stack allocated memory
    free(stack);
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
SDL_Surface* Blob(SDL_Surface* img, int* size_max)
{
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
	if (pixels==NULL||blob ==NULL)
	{
		warn("Not enough storage for this operation");
	}

	//area_max will be the max size of a black blob while size
	//is the size of the current blob
	int max_size = 0;
	int lim = 1;

	//Basic colors
	Uint32 whitePx = SDL_MapRGB(src->format, 255, 255, 255);

	//Runs through the image to find the biggest blob
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			int pxVal = getPixelGrayScale(pixels[i*width +  j]);

			if (pxVal<255 && blob[i*width+j]==0)
			{
				Fill(src, pixels, j, i, blob, lim);

				if (size_blob>max_size)
				{
					max = lim;
					max_size = size_blob;
				}

				size_blob = 0;
				lim++;
			}
		}
	}

	//Runs through the image again and repaint the smaller blobs
	for(int i = 0; i < src -> h; i++)
	{
		for(int j = 0; j < src -> w; j++)
		{
			int pxVal = getPixelGrayScale(pixels[i*width +  j]);

			if (blob[i*width+j]!=max && pxVal<255)
			{
				pixels[i*width+ j] = whitePx;
			}
		}
	}

	//Modifies pointer size, free the allocated and returns the surface
	*size_max = max_size;
	free(blob);

	return src;
}

double Distance(Pointx_y p1, Pointx_y p2)
{
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

int isSquare_Blob(Pointx_y p1, Pointx_y p2, Pointx_y p3, Pointx_y p4)
{
    // Calculation of distances between points
    double d12 = Distance(p1, p2);
    double d13 = Distance(p1, p3);
    double d14 = Distance(p1, p4);
    double d23 = Distance(p2, p3);
    double d24 = Distance(p2, p4);
    double d34 = Distance(p3, p4);

	// Verification of side lengths
    double errorMargin = 0.75;
    if (fabs(d12 - d13) / d12 > errorMargin || fabs(d12 - d14) / d12 >
			errorMargin || fabs(d12 - d23) / d12 > errorMargin ||

        fabs(d12 - d24) / d12 > errorMargin || fabs(d12 - d34) / d12 >
			errorMargin)
	{
        return 0; // The sides are not equal
    }

    // Checking right angles
    double diag1 = Distance(p1, p3);
    double diag2 = Distance(p2, p4);
    if (fabs(diag1 * diag1 + d12 * d12 - d13 * d13) / (2 * diag1 * d12) >
			errorMargin ||
        fabs(diag2 * diag2 + d12 * d12 - d24 * d24) / (2 * diag2 * d12) >
			errorMargin) {
        return 0; // The angles are not straight
    }

    return 1; // The conditions are met, it is a square
}

SDL_Surface* Remove_Blob(SDL_Surface* image, SDL_Surface* image_blob)
{
	int w = image->w;
	int h = image->h;

	SDL_Surface *src = SDL_ConvertSurfaceFormat
						(
							image,
							SDL_PIXELFORMAT_ARGB8888,
							0
						);

	Uint32* pixels_src = src->pixels;
	Uint32* pixels_blob = image_blob->pixels;

	//Basic colors
	Uint32 whitePx = SDL_MapRGB(image->format, 255, 255, 255);

	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			int pxVal = getPixelGrayScale(pixels_blob[i*w +  j]);

			if(pxVal < 255)
			{
				pixels_src[i*w + j] = whitePx;
			}
		}
	}
	free(image_blob);

	return src;
}