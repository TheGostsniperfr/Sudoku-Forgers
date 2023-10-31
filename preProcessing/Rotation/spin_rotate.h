#ifndef SPIN_ROTATE_H
#define SPIN_ROTATE_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#include "rotation.h"

void draw_rotated(SDL_Renderer* renderer, SDL_Texture* texture);
void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Surface* image);
void Spin_rotate(SDL_Surface* image);

#endif
