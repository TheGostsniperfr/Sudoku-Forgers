#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../preProcessing/SDL_Function/sdlFunction.h"
#include "../solver/sudoku_solver.h"

#define GRID_WIDTH 1000
#define GRID_HEIGHT 1000

//wirdth of each border of the sudoku grid in px
#define WIDTH_BORDER 15
#define RATIO_BORDER 0.5
#define NB_SUB_square 3


void createGrid(SDL_Surface* img, int x_0, int y_0, int w, double widthBorder, int depthLevel, Uint32 lineColor, Uint32 bgColor){
    if(depthLevel <= 0){
        return;
    }

    //Draw line
    drawRect(img, x_0, y_0, w, w, lineColor);

    //Draw bg
    drawRect(img, x_0+widthBorder, y_0+widthBorder, w-2*widthBorder, w-2*widthBorder, bgColor);


    int new_widthBorder = (int) (widthBorder * RATIO_BORDER);
    int new_w = w / NB_SUB_square ;


    for (int x = 0; x < NB_SUB_square; x++)
    {
        for (int y = 0; y < NB_SUB_square; y++)
        {
            
            int new_x_0 = x_0 + widthBorder - new_widthBorder + x * (new_w - new_widthBorder);
            int new_y_0 = y_0 + widthBorder - new_widthBorder + y * (new_w - new_widthBorder);

            

            createGrid(img, new_x_0, new_y_0, new_w, new_widthBorder, depthLevel-1, lineColor, bgColor);
        }        
    }
}


SDL_Surface* createOutputGrid(const char *path){
    int testGrid[9][9];

    if(loadGrid(path, testGrid) == 1){
        errx(1, "Create output grid can't to load grid!");
    }

    SDL_Surface* outputImg = SDL_CreateRGBSurfaceWithFormat(0, GRID_WIDTH, GRID_HEIGHT, 32, SDL_PIXELFORMAT_ABGR8888);

    //crete background of the grid
    Uint32 whitePixel = SDL_MapRGBA(outputImg->format, 255, 255, 255, 0);
    SDL_Rect rect =  {0, 0, GRID_WIDTH ,GRID_HEIGHT};
    SDL_FillRect(outputImg, &rect, whitePixel);

    Uint32 blackPixel = SDL_MapRGBA(outputImg->format, 0, 0, 0, 0);


    createGrid(outputImg, 0,0, outputImg->w, WIDTH_BORDER, 3, blackPixel, whitePixel);

    
    saveImg(outputImg, "outputGrid.jpg");

    return outputImg;
}

