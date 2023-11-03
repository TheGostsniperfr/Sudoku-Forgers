#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"


/***************************************************************
 *  Function Bilinear_Interpolation:
 *
 *  Fonction to get a perfect image of the grid from the old image
 *  after the blob filter
 *
 *  @input :
 *      - image (SDL_Surface*) : a surface from many filter
 *      - output_width (int) : the width of the image that will be returned
 *      - output_height (int) : the height of the image that will be returned
 *  @output :
 *      - result (SDL_Surface*) : the new surface with a new width and height
***************************************************************/
SDL_Surface* Bilinear_Interpolation(SDL_Surface* image, int output_width,
	int output_height)
{
	SDL_Surface* output_image = SDL_CreateRGBSurface(0, output_width,
		output_height, 32,0,0,0,0);

	double input_width = image->w;
	double input_height = image->h;

	for (int i = 0; i < output_height; i++)
	{
		for (int j = 0; j < output_width; j++)
		{
			float gx = j / (float)(output_width) * input_width;
			float gy = i / (float)(output_height) * input_height;

			int gxi = (int)gx;
			int gyi = (int)gy;

			Uint32* input_pixels = (Uint32*)image->pixels;

			Uint32 ul = input_pixels[floor(gy) * output_image->w + floor(gx)];
			Uint32 ur = input_pixels[ceil(gy) * output_image->w + floor(gx)];
			Uint32 ll = input_pixels[floor(gy) * output_image->w + ceil(gx)];
			Uint32 lr = input_pixels[ceil(gy) * output_image->w + ceil(gx)];

			Uint8 ul_r, ul_g, ul_b, ul_a;
			SDL_GetRGBA(ul, image->format, &ul_r, &ul_g, &ul_b, &ul_a);

			Uint8 ur_r, ur_g, ur_b, ur_a;
			SDL_GetRGBA(ur, image->format, &ur_r, &ur_g, &ur_b, &ur_a);

			Uint8 ll_r, ll_g, ll_b, ll_a;
			SDL_GetRGBA(ll, image->format, &ll_r, &ll_g, &ll_b, &ll_a);

			Uint8 lr_r, lr_g, lr_b, lr_a;
			SDL_GetRGBA(lr, image->format, &lr_r, &lr_g, &lr_b, &lr_a);

			float gx_p = gx - (float)gxi;
			float gy_p = gy - (float)gyi;

			Uint8 newR, newG, newB, newA;

			newR = (Uint8)((gx_p*(float)ul_r +(float)(1-gx_p)*(float)ur_r)*gy_p
			+ (float)(1-gy_p)*(gx_p*(float)ll_r + (1-gx_p)*(float)lr_r));
			newG = (Uint8)((gx_p*(float)ul_g +(float)(1-gx_p)*(float)ur_g)*gy_p
			+ (float)(1-gy_p)*(gx_p*(float)ll_g + (1-gx_p)*(float)lr_g));
			newB = (Uint8)((gx_p*(float)ul_b +(float)(1-gx_p)*(float)ur_b)*gy_p
			+ (float)(1-gy_p)*(gx_p*(float)ll_b + (1-gx_p)*(float)lr_b));
			newA = (Uint8)((gx_p*(float)ul_a +(float)(1-gx_p)*(float)ur_a)*gy_p
			+ (float)(1-gy_p)*(gx_p*(float)ll_a + (1-gx_p)*(float)lr_a));

			Uint32 newpixel = SDL_MapRGBA(output_image->format, newR, newG,
			newB, newA);

			Uint32* output_pixels = (Uint32*)output_image->pixels;
			output_pixels[i * output_image->w + i] = newpixel;
		}
	}

	return output_image;
}