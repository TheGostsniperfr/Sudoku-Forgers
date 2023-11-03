#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/BlobDetection/BlobDetection.h"


typedef struct GridCell {
	(int, int) position;
	SDL_Surface* image;
	int size = 28;
}GridCell;


GridCell* noise_detetcion(SDL_Surface** images)
{
	GridCell* clean_img =  malloc(9 * sizeof(GridCell));
	for (size_t i = 0; i < 9; i++)
	{
		Blob(images[i], 1);
	}
}