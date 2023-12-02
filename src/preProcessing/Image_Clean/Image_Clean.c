#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/BlobDetection/BlobDetection.h"
#include "preProcessing/CaseDetection/case_detection.h"

void reversedBW(SDL_Surface* image)
{
	SDL_LockSurface(image);

	Uint32* pixels = (Uint32*)image->pixels;

	for (int y = 0; y < image->h; y++)
        {
        for (int x = 0; x < image->w; x++)
        {
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

void clean_edge(SDL_Surface* image) {   
    int count_px;
    Uint8 r, g, b;
    int Threshold = (image->w)/2;

    SDL_LockSurface(image);

    for (int y = 0; y < image->h; ++y) {
        count_px = 0;
        for (int x = 0; x < image->w; ++x) {   
            Uint32* pixel = (Uint32*)((Uint8*)image->pixels + y * image->pitch + x * sizeof(Uint32));
            SDL_GetRGB(*pixel, image->format, &r, &g, &b);
            if (r < 100 && g < 100 && b < 100)
                count_px++;
        }
        if (count_px > Threshold) {
            for (int x = 0; x < image->w; ++x) {
                Uint32* pixel = (Uint32*)((Uint8*)image->pixels + y * image->pitch + x * sizeof(Uint32));
                *pixel = SDL_MapRGB(image->format, 255, 255, 255);
            }
        }
    }

    for (int x = 0; x < image->w; ++x) {
        count_px = 0;
        for (int y = 0; y < image->h; ++y) {
            Uint32* pixel = (Uint32*)((Uint8*)image->pixels + y * image->pitch + x * sizeof(Uint32));
            SDL_GetRGB(*pixel, image->format, &r, &g, &b);
            if (r < 100 && g < 100 && b < 100)
                count_px++;
        }
        if (count_px > Threshold) {
            for (int y = 0; y < image->h; ++y) {
                Uint32* pixel = (Uint32*)((Uint8*)image->pixels + y * image->pitch + x * sizeof(Uint32));
                *pixel = SDL_MapRGB(image->format, 255, 255, 255);
            }
        }
    }

    SDL_UnlockSurface(image);
}

void clean_all(SDL_Surface* image) 
{
    SDL_LockSurface(image);

    int width = image->w;
    int height = image->h;

    for (int y = 0; y < height; ++y) 
    {
        for (int x = 0; x < width; ++x) 
        {
            Uint32* pixel = (Uint32*)((Uint8*)image->pixels + y * image->pitch + x * sizeof(Uint32));
            *pixel = SDL_MapRGB(image->format, 0, 0, 0);
        }
    }

    SDL_UnlockSurface(image);
}

void Image_Clean(GridCell* images)
{
	int size = 0;
    for (size_t i = 0; i < 81; i++)
    {
        clean_edge(images[i].image);
        images[i].image = Blob(images[i].image, &size);
        images[i].label = size;
        if(size <= 40)
        {
            clean_all(images[i].image);
            images[i].isDigit = 0;
        }
        else
        {
	        reversedBW(images[i].image);
            images[i].isDigit = 1;
        }
    }
}