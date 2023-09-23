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
#define SMALL_BORDER 2
#define LARGE_BORDER 8

void createSquare(SDL_Surface* img, int x0, int y0, int w, int h, Uint32 pixelLine, Uint32 pixelBg, int lW){
    

    drawRect(img, x0, y0, w, h, pixelLine);

    drawRect(img, x0+lW, y0+lW, w-2*lW, h-2*lW, pixelBg);

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



    

    int squareWidth = GRID_HEIGHT / 9;

    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            createSquare(outputImg, x * squareWidth, y * squareWidth, squareWidth, squareWidth, blackPixel, whitePixel, SMALL_BORDER);
        }
        
    }
    
    
    
    
    

    
    saveImg(outputImg, "outputGrid.jpg");

    return outputImg;
}

