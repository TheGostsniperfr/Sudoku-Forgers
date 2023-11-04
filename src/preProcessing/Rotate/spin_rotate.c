#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

#include "preProcessing/Rotate/rotate.h"

void draw_rotated(SDL_Renderer* renderer, SDL_Texture* texture)
{
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void event_loop(SDL_Renderer* renderer, SDL_Texture* texture,
	SDL_Surface* image)
{
	SDL_Event event;
	draw_rotated(renderer, texture);

	int angle = 0;
	SDL_Surface* img;

	while (1)
	{

		// Waits for an event.
		SDL_WaitEvent(&event);

		switch (event.type)
		{
			// If the "quit" button is pushed, ends the event loop.
			case SDL_QUIT:
				return;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					draw_rotated(renderer, texture);
				}
				break;
			case SDL_MOUSEWHEEL:
				if(event.wheel.y > 0)
				{
					angle += 1;
					img = Rotated_image(image, angle);
					texture = SDL_CreateTextureFromSurface(renderer, img);
					draw_rotated(renderer, texture);
				}
				else
				{
					angle -= 1;
					img = Rotated_image(image, angle);
					texture = SDL_CreateTextureFromSurface(renderer, img);
					draw_rotated(renderer, texture);
				}
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_UP:
						angle += 10;
						img = Rotated_image(image, angle);
						texture = SDL_CreateTextureFromSurface(renderer, img);
						draw_rotated(renderer, texture);
						break;
					case SDLK_DOWN:
						angle -= 10;
						img = Rotated_image(image, angle);
						texture = SDL_CreateTextureFromSurface(renderer, img);
						draw_rotated(renderer, texture);
						break;
     			}
				break;
		}
	}
}

void Spin_rotate(SDL_Surface* image)
{
	// Initializes the SDL.
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	// Creates a window.
	SDL_Window* window = SDL_CreateWindow("Automatic rotated image", 0, 0, 100,
		 100, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	// Creates a renderer.
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	//Create a texture from the image
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	if (texture == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	// Gets the width and the height of the texture.
	int w, h;
	if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());

	// - Resize the window according to the size of the image.
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_SetWindowSize(window, w, h);

	// Dispatches the events.
	event_loop(renderer, texture, image);

	// Destroys the objects.
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}