#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../preProcessing/SDL_Function/sdlFunction.h"
#include "../sudokuSolver/sudoku_solver.h"
#include <SDL_ttf.h>
#include "../sudokuAux/sudokuUtil.h"


//path to file :

//path to grid (input and solved)
#define PATH_INPUT_GRID "../data/input_matrix/input_matrix16x162"
#define PATH_SOLVED_GRID "grid.result"

//path to digit font
#define PATH_FONT "../data/font/MontserratMedium.ttf"


#define GRID_WIDTH 1000
#define GRID_HEIGHT 1000

//width of each border of the sudoku grid in px
#define WIDTH_BORDER 15
#define RATIO_BORDER 0.5

#define FONT_RATIO 0.8

typedef struct GridPara{
    SDL_Surface* img;
    double x_0;
    double y_0;
    double w;
    double widthBorder;
    int depthLevel;
    Uint32 digitColor;
    int id;
    Uint32 lineColor;
    Uint32 bgColor;
    TTF_Font* font;
    int gS;
} GridPara;


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

void createGrid(GridPara gP, SudokuGrid defaultSG, SudokuGrid solvedSG){

    if(gP.depthLevel <= 0){
        return;
    }

    //Draw line
    drawRect(
            gP.img,
            gP.x_0,
            gP.y_0,
            gP.w,
            gP.w,
            gP.lineColor
        );

    //Draw bg
    drawRect(
            gP.img,
            (int)gP.x_0+gP.widthBorder,
            (int)gP.y_0+gP.widthBorder,
            (int)gP.w-2*gP.widthBorder,
            (int)gP.w-2*gP.widthBorder,
            gP.bgColor
        );


    //write digit
    if(gP.depthLevel == 1){
        char chiffreStr[2];
        snprintf(chiffreStr, 2, "%c", '0' + gP.id);

        Uint8 r,g,b;
        SDL_GetRGB(gP.digitColor, gP.img->format, &r, &g, &b);

        SDL_Color textColor = {r, g, b, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(gP.font, chiffreStr, textColor);

        if (!textSurface) {
            errx(1, "Error to create text surface.\n");
            return;
        }

        int textX = (gP.x_0 + gP.widthBorder + (gP.w - 2 * gP.widthBorder) / 2) - textSurface->w / 2;
        int textY = (gP.y_0 + gP.widthBorder + (gP.w - 2 * gP.widthBorder) / 2) - textSurface->h / 2;


        SDL_Rect dstRect = {textX, textY, textSurface->w, textSurface->h};

        SDL_BlitSurface(textSurface, NULL, gP.img, &dstRect);
        SDL_FreeSurface(textSurface);
        return;
    }

    //start to create sub square / recurtion
    double new_widthBorder = (gP.widthBorder * RATIO_BORDER);
    double new_w = gP.w / gP.gS ;
    gP.depthLevel--;

    for (int x = 0; x < gP.gS; x++)
    {
        for (int y = 0; y < gP.gS; y++)
        {
            double new_x_0 =   gP.x_0 +
                            gP.widthBorder -
                            new_widthBorder +
                            x * (new_w - new_widthBorder);

            double new_y_0 =   gP.y_0 +
                            gP.widthBorder -
                            new_widthBorder +
                            y * (new_w - new_widthBorder);

            int new_id = gP.id;

            if(gP.depthLevel == 2){
                new_id = x + y * gP.gS;
            }

            if(gP.depthLevel == 1){
                //get digit to write in grid with diff color for solved digit

                new_id =  x + gP.gS * y;

                if(defaultSG.grid[(int)((gP.id/gP.gS)*gP.gS + y)][(int)((gP.id%gP.gS)*gP.gS + x)] == -1){
                    new_id = solvedSG.grid[(int)((gP.id/gP.gS)*gP.gS + y)][(int)((gP.id%gP.gS)*gP.gS + x)];

                    GridPara tempGP = gP;
                    tempGP.x_0 = new_x_0;
                    tempGP.y_0 = new_y_0;
                    tempGP.w = new_w;
                    tempGP.widthBorder = new_widthBorder;
                    tempGP.id = new_id;

                    createGrid(tempGP, defaultSG, solvedSG);

                    continue;

                }else{
                    new_id = defaultSG.grid[(int)((gP.id/gP.gS)*gP.gS + y)][(int)((gP.id%gP.gS)*gP.gS + x)];

                    GridPara tempGP = gP;
                    tempGP.x_0 = new_x_0;
                    tempGP.y_0 = new_y_0;
                    tempGP.w = new_w;
                    tempGP.widthBorder = new_widthBorder;
                    tempGP.digitColor = gP.lineColor;
                    tempGP.id = new_id;

                    createGrid(tempGP, defaultSG, solvedSG);
                    continue;
                }
            }

            GridPara tempGP = gP;
            tempGP.x_0 = new_x_0;
            tempGP.y_0 = new_y_0;
            tempGP.w = new_w;
            tempGP.widthBorder = new_widthBorder;
            tempGP.id = new_id;

            createGrid(tempGP, defaultSG, solvedSG);
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

void gridBuilder(SDL_Surface* img, TTF_Font* font, SudokuGrid defaultSG,
                    SudokuGrid solvedSG){

    Uint32 bgColor = SDL_MapRGBA(img->format, 255, 255, 255, 0);

    //create background of the grid
    SDL_Rect rect =  {0, 0, GRID_WIDTH ,GRID_HEIGHT};
    SDL_FillRect(img, &rect, bgColor);

    //Color of the digit (default : red)
    Uint32 digitColor = SDL_MapRGBA(img->format, 255, 0, 0, 0);


    //start recursion of the grid build

    GridPara gP = {
        .img = img,
        .x_0 = 0,
        .y_0 = 0,
        .w = img->w,
        .widthBorder = WIDTH_BORDER,
        .depthLevel = 3,
        .digitColor = digitColor,
        .id = 0,
        .lineColor = SDL_MapRGBA(img->format, 0, 0, 0, 0),
        .bgColor = bgColor,
        .font = font,
        .gS = defaultSG.gS == 9 ? 3 : 4
    };


    createGrid(gP, defaultSG, solvedSG);
}


/***************************************************************
 *  Function gridBuilder:
 *
 *  main function to create all the final grid
 *
 *  @output :
 *      - img (SDL_Surface*) : final img of the final grid
***************************************************************/
SDL_Surface* createOutputGrid(SudokuGrid defaultSG, SudokuGrid solvedSG){

    //Init TTF
    if (TTF_Init() < 0) {
        errx(EXIT_FAILURE ,"⚠️ Error to init TTF. ⚠️");
    }


    //Load font to write digit
    TTF_Font* font = TTF_OpenFont(PATH_FONT, GRID_WIDTH/9 * FONT_RATIO);
    if (!font) {
        errx(EXIT_FAILURE, "⚠️ Error to load font.ttf. ⚠️");
    }


    //Create output surface
    SDL_Surface* outputImg = SDL_CreateRGBSurfaceWithFormat(0, GRID_WIDTH, GRID_HEIGHT, 32, SDL_PIXELFORMAT_ABGR8888);


    //build representation grid of sudokuGrid
    gridBuilder(outputImg, font, defaultSG, solvedSG);



    //Close libs
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return outputImg;
}