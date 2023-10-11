#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../preProcessing/SDL_Function/sdlFunction.h"
#include "../solver/sudoku_solver.h"
#include <SDL_ttf.h>


//path to file :

//path to grid (input and solved)
#define PATH_INPUT_GRID "../data/input_matrix/input_matrix.result"
#define PATH_SOLVED_GRID "grid.result"

//path to digit font
#define PATH_FONT "../data/font/MontserratMedium.ttf"


#define GRID_WIDTH 1000
#define GRID_HEIGHT 1000

//width of each border of the sudoku grid in px
#define WIDTH_BORDER 15
#define RATIO_BORDER 0.5
#define NB_SUB_square 3

#define FONT_RATIO 0.8

static int** defaultGrid;
static int** solvedGrid;

static Uint32 lineColor;
static Uint32 bgColor;

static TTF_Font* font;


/***************************************************************
 *  Function createGrid: 
 *
 *  Recursif function to create all square with digit of the grid 
 *
 *  @intput :
 *      - img (SDL_Surface*) : input image (only background)
 *      - x_0 (int) : x coordonate to start to draw
 *      - y_0 (int) : y coordonate to start to draw
 *      - w (int) : width of the square to draw
 *      - widthBorder (double) : witdh of sqare's borders to draw
 *      - depthLevel (int) : level of the recursion (nb of recursion inside)
 *      - digitColor (Uint32) : color of the digit 
 *      - id (int) : id of the current case (use to calculate x,y in the grid)
***************************************************************/

void createGrid(SDL_Surface* img, int x_0, int y_0, int w, double widthBorder, int depthLevel, Uint32 digitColor, int id){
    
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
        return;
    }

    //start to create sub square / recurtion
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
                    createGrid(img, new_x_0, new_y_0, new_w, new_widthBorder, depthLevel, digitColor, new_id);
                    continue;

                }else{
                    new_id = defaultGrid[(id/3)*3 + y][(id%3)*3 + x];
                    createGrid(img, new_x_0, new_y_0, new_w, new_widthBorder, depthLevel, lineColor, new_id);
                    continue;
                }
            }

            createGrid(img, new_x_0, new_y_0, new_w, new_widthBorder, depthLevel, digitColor, new_id);
        }        
    }
    
}


/***************************************************************
 *  Function gridBuilder: 
 *
 *  hat function to create grid
 *
 *  @input :
 *      - img (SDL_Surface*) : input surface to build grid
***************************************************************/

void gridBuilder(SDL_Surface* img){
    
    //base color
    bgColor = SDL_MapRGBA(img->format, 255, 255, 255, 0);
    lineColor = SDL_MapRGBA(img->format, 0, 0, 0, 0);


    //create background of the grid
    SDL_Rect rect =  {0, 0, GRID_WIDTH ,GRID_HEIGHT};
    SDL_FillRect(img, &rect, bgColor);

    //Color of the digit (default : red)
    Uint32 digitColor = SDL_MapRGBA(img->format, 255, 0, 0, 0);


    //start recursion of the grid build
    createGrid(img, 0,0, img->w, WIDTH_BORDER, 3, digitColor, 0);
}


/***************************************************************
 *  Function gridBuilder: 
 *
 *  main function to create all the final grid
 *
 *  @output :
 *      - img (SDL_Surface*) : final img of the final grid
***************************************************************/
SDL_Surface* createOutputGrid(int gS){

    //alloc grid
    defaultGrid = (int**)calloc(gS, sizeof(int*));
    for(int i = 0; i < gS; i++){
        defaultGrid[i] = (int*)calloc(gS, sizeof(int));
    }

    solvedGrid = (int**)calloc(gS, sizeof(int*));
    for(int i = 0; i < gS; i++){
        solvedGrid[i] = (int*)calloc(gS, sizeof(int));
    }

    //Load sudoku grids (file.result)
    if(loadGrid(PATH_INPUT_GRID, defaultGrid, gS) == 1){
        errx(1, "Error can't to load default grid.");
    }
    
    if(loadGrid(PATH_SOLVED_GRID, solvedGrid, gS) == 1){
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
    font = TTF_OpenFont(PATH_FONT, GRID_WIDTH/9 * FONT_RATIO);
    if (!font) {
        errx(1, "Error to load font.ttf.\n");        
    }


    //Create output surface 
    SDL_Surface* outputImg = SDL_CreateRGBSurfaceWithFormat(0, GRID_WIDTH, GRID_HEIGHT, 32, SDL_PIXELFORMAT_ABGR8888);


    //build representation grid of sudokuGrid
    gridBuilder(outputImg);



    //Close libs
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return outputImg;
}