#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/BlobDetection/BlobDetection.h"

void noise_detetcion(SDL_Surface** images)
{
	for (size_t i = 0; i < 9; i++)
	{
			Blob(images[i]);
	}
}