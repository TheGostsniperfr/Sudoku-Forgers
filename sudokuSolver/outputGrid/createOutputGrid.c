#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../preProcessing/SDL_Function/sdlFunction.h"
#include "../solver/sudoku_solver.h"
#include <SDL_ttf.h>

#define GRID_WIDTH 1000
#define GRID_HEIGHT 1000

//width of each border of the sudoku grid in px
#define WIDTH_BORDER 15
#define RATIO_BORDER 0.5
#define NB_SUB_square 3

#define FONT_RATIO 0.8

static int defaultGrid[9][9];
static int solvedGrid[9][9];

static Uint32 lineColor;
static Uint32 bgColor;


void createGrid(SDL_Surface* img, int x_0, int y_0, int w, double widthBorder, int depthLevel, Uint32 digitColor, TTF_Font* font, int id){
    
    if(depthLevel <= 0){
        return;
    }

    //Draw line
    drawRect(img, x_0, y_0, w, w, lineColor);

    //Draw bg
    drawRect(img, x_0+widthBorder, y_0+widthBorder, w-2*widthBorder, w-2*widthBorder, bgColor);


    //write digit
    if(depthLevel == 1){
        char chiffreStr[2];
        snprintf(chiffreStr, 2, "%c", '0' + id);  

        Uint8 r,g,b;
        SDL_GetRGB(digitColor, img->format, &r, &g, &b);

        SDL_Color textColor = {r, g, b, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, chiffreStr, textColor);

        if (!textSurface) {
            errx(1, "Error to create text surface.\n");
            return;
        }

        int textX = (x_0 + widthBorder + (w - 2 * widthBorder) / 2) - textSurface->w / 2;
        int textY = (y_0 + widthBorder + (w - 2 * widthBorder) / 2) - textSurface->h / 2;
        

        SDL_Rect dstRect = {textX, textY, textSurface->w, textSurface->h};

        SDL_BlitSurface(textSurface, NULL, img, &dstRect);
        SDL_FreeSurface(textSurface);
    }



    int new_widthBorder = (int) (widthBorder * RATIO_BORDER);
    int new_w = w / NB_SUB_square ;

    depthLevel--;

    for (int x = 0; x < NB_SUB_square; x++)
    {
        for (int y = 0; y < NB_SUB_square; y++)
        {
            
            int new_x_0 = x_0 + widthBorder - new_widthBorder + x * (new_w - new_widthBorder);
            int new_y_0 = y_0 + widthBorder - new_widthBorder + y * (new_w - new_widthBorder);

            int new_id = id;

            if(depthLevel == 2){
                new_id = x + y * 3;
            }

            if(depthLevel == 1){
                //get digit to write in grid with diff color for solved digit

                new_id =  x + 3 * y;                

                if(defaultGrid[(id/3)*3 + y][(id%3)*3 + x] == 0){
                    new_id = solvedGrid[(id/3)*3 + y][(id%3)*3 + x];
                }else{
                    new_id = defaultGrid[(id/3)*3 + y][(id%3)*3 + x];  
                    digitColor = lineColor; 
                    
                }
            }

            createGrid(img, new_x_0, new_y_0, new_w, new_widthBorder, depthLevel, digitColor ,font, new_id);
        }        
    }
    
}



void gridBuilder(SDL_Surface* img, TTF_Font* font){
    
    //base color
    bgColor = SDL_MapRGBA(img->format, 255, 255, 255, 0);
    lineColor = SDL_MapRGBA(img->format, 0, 0, 0, 0);


    //crete background of the grid
    SDL_Rect rect =  {0, 0, GRID_WIDTH ,GRID_HEIGHT};
    SDL_FillRect(img, &rect, bgColor);

    Uint32 digitColor = SDL_MapRGBA(img->format, 255, 0, 0, 0);


    //start recursion of the grid build
    createGrid(img, 0,0, img->w, WIDTH_BORDER, 3, digitColor, font, 0);
}


SDL_Surface* createOutputGrid(const char *path){

    //Load sudoku grids (file.result)

    if(loadGrid("/media/prog/OCR-Sudoku-Solver/data/input_matrix/input_matrix.result", defaultGrid) == 1){
        errx(1, "Error can't to load default grid.");
    }
    
    if(loadGrid(path, solvedGrid) == 1){
        errx(1, "Error can't to load solved grid.");
    }


    //Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        errx(1,"Error to init SDL.\n");        
    }


    //Init TTF 
    if (TTF_Init() < 0) {
        errx(1,"Error to init TTF.\n");
    }


    //Load font to write digit
    TTF_Font* font = TTF_OpenFont("/media/prog/OCR-Sudoku-Solver/data/font/MontserratMedium.ttf", GRID_WIDTH/9 * FONT_RATIO);
    if (!font) {
        errx(1, "Error to load font.ttf.\n");        
    }


    //Create output surface 
    SDL_Surface* outputImg = SDL_CreateRGBSurfaceWithFormat(0, GRID_WIDTH, GRID_HEIGHT, 32, SDL_PIXELFORMAT_ABGR8888);


    //build representation grid of sudokuGrid
    gridBuilder(outputImg, font);

    //save img of the representation
    saveImg(outputImg, "outputGrid.jpg");


    //Close libs
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return outputImg;
}

