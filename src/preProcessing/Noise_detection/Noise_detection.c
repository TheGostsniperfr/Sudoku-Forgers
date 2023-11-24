#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/BlobDetection/BlobDetection.h"
#include "preProcessing/CaseDetection/case_detection.h"

void reversedBW(SDL_Surface* image)
{
	SDL_LockSurface(image);

	Uint32* pixels = (Uint32*)image->pixels;

	for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            Uint32 pixel = pixels[y * image->w + x];
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, image->format, &r, &g, &b, &a);

            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            pixels[y * image->w + x] = SDL_MapRGBA(image->format, r, g, b, a);
        }
    }

    SDL_UnlockSurface(image);
}

GridCell* NoiseDetection(GridCell* images)
{
	GridCell* clean_img =  malloc(81 * sizeof(GridCell));
	for (size_t i = 0; i < 81; i++)
	{
		int maxSize = 0;
		//saveImg(images[i].image, ("Img_%s_PreClean", i));
		clean_img[i].image = Blob(images[i].image, &maxSize);
		//saveImg(clean_img[i].image, ("Img_%s_PostClean", i));
	}
	return clean_img;
}