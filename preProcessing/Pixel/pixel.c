#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"



Uint8 getPixel8(SDL_Surface* surface, int x, int y) {
	int Bytes_par_pixel = surface->format->BytesPerPixel;
	Uint8 *pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * Bytes_par_pixel;
	return *pixel;
}


void putPixel(SDL_Surface*surface, int x, int y, uint32_t pixel)
{
	int bpp = surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(uint16_t *)p = pixel;
		break;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

uint32_t getPixel32_t(SDL_Surface *surface, int x, int y)
{
	return *(uint32_t *) ((uint8_t *)
			surface->pixels
				+ y * surface->pitch
				+ x * surface->format->BytesPerPixel);
}