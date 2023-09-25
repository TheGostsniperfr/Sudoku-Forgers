#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "math.h"

#include "pixel.h"
#include "coins_detecter.h"

// Define Pi
#define M_PI 3.14159265358979323846


/* double Find_angle_to_Rotate(SDL_Surface* image)
{
    //Initialize angle and the array of the corners
    double angle = 0;
    int points[8];

    //Get the corners of the image
    detecterCoins(image, points);

    //Get the coordonates of the lower left corner
    int ll_x = points[2];
    int ll_y = points[3];
    //printf("(%d,%d) lower left coordonates\n", ll_x, ll_y);

    //Get the coordonates of the lower right corner
    int lr_x = points[4];
    int lr_y = points[5];
    //printf("(%d,%d) lower right coordonates\n", lr_x, lr_y);

    //Get the length and the width between these two corners
    int width = ll_y - lr_y;   
    int length = ll_x - lr_x;
    //printf("%d and %d ditance segment\n", width, length);

    //Get the angle to rotate the image
    angle = atan(round(length/width));
 
    //return the round of the angle
    return angle;
} */


SDL_Surface* Rotated_image(SDL_Surface* image, double angle)
{
    //Get the dimension of the image
    int w = image->w;
    int h = image->h;

    //Get the center of the image
    int center_x = w/2;
    int center_y = h/2;

    //Create a new image for the rotated image
    SDL_Surface* rotated_image = SDL_CreateRGBSurface(0, w, h, 32,0,0,0,0);

    //Get angle from degree to radian
    double rad_angle = angle * (M_PI / 180);

    //Get the angles for sin and tan
    double angle_tan = tan(rad_angle/2);
    double angle_sin = sin(rad_angle);


    //Travel into the image to rotate it
    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            int new_x;
            int new_y;

            int x = i - center_x;
            int y = j - center_y;

            //step 1
            new_x = round(x - y*angle_tan);
            new_y = y;

            //step 2
            new_y = round(new_x *angle_sin+new_y);

            //step 3
            new_x = round(new_x-new_y*angle_tan);

            new_y += center_y;
            new_x += center_x;

            if(new_x >= 0 && new_x < w && new_y >= 0 && new_y < h)
            {
                Uint32 pixel = getPixel32_t(image, i, j);
                putPixel(rotated_image, new_x, new_y, pixel);
            }
        }
    }

    SDL_FreeSurface(image);
    return rotated_image;
}