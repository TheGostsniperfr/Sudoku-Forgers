#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/BlobDetection/BlobDetection.h"
#include "preProcessing/CaseDetection/case_detection.h"


GridCell* noise_detetcion(GridCell* images)
{
	GridCell* clean_img =  malloc(81 * sizeof(GridCell));
	for (size_t i = 0; i < 81; i++)
	{
		clean_img[i].image = Blob(images[i].image, (int*)1);
	}
	return clean_img;
}