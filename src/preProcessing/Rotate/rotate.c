#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/Matrix/mat.h"

// Define Pi
#define M_PI 3.14159265358979323846

SDL_Surface* Rotated_image(SDL_Surface* image, double angle)
{
    // Get the dimension of the image
    int w = image->w;
    int h = image->h;

    // Get the center of the image
    int center_x = w / 2;
    int center_y = h / 2;

    // Create a new image for the rotated image
    SDL_Surface* rotated_image = SDL_CreateRGBSurfaceWithFormat(
        0,
        w,
        h,
        32,
        SDL_PIXELFORMAT_ABGR8888);

    Uint32* rotated_pixels = (Uint32*)rotated_image->pixels;

    // Basic colors
    Uint32 whitePx = SDL_MapRGB(image->format, 255, 255, 255);

    for (int i = 0; i < rotated_image->h; i++)
    {
        for (int j = 0; j < rotated_image->w; j++)
        {
            rotated_pixels[i * rotated_image->w + j] = whitePx;
        }
    }
	//Get angle from degree to radian
    double rad_angle = angle * (M_PI / 180);

	//Get the angles for sin and cose
	double angle_sin = sin(rad_angle);
    double angle_cos = cos(rad_angle);

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            int new_x =
				(i - center_x)
				* angle_cos
				- (j - center_y)
				* angle_sin
				+ center_x;

            int new_y =
				(i - center_x)
				* angle_sin
				+ (j - center_y)
				* angle_cos
				+ center_y;

            Uint32* pixels = (Uint32*)image->pixels;

            if (new_x >= 0 && new_x < w && new_y >= 0 && new_y < h)
            {
                Uint32 pixel = pixels[j * image->w + i];
                rotated_pixels[new_y * rotated_image->w + new_x] = pixel;
            }
        }
    }

    return rotated_image;
}

void Save_RotatedImg(SDL_Surface* image, double angle)
{
	SDL_Surface* rotated_img = Rotated_image(image, angle);
	saveImg(rotated_img, "Rotated_Img.jpg");
}
