#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>

#include "preProcessing/SDL_Function/sdlFunction.h"
#include "sudokuSolver/sudokuSolver/sudoku_solver.h"
#include "sudokuSolver/sudokuAux/sudokuUtil.h"


//path to file :

//path to grid (input and solved)

//path to digit font
#define PATH_FONT "../../data/font/MontserratMedium.ttf"


typedef struct SubGridPara{
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
    Uint32 bgColor2;
} SubGridPara;


typedef struct GridPara {
    double fontRatio;
    double borderRatio;
    double gridPxSize;
    double widthBorder;


}GridPara;

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

void createGrid(SubGridPara sGP, GridPara gP, SudokuGrid defaultSG,
                    SudokuGrid solvedSG){

    if(sGP.depthLevel == 1){
        //return;
    }

    if(sGP.depthLevel <= 0){
        return;
    }

    //Draw line
    drawRect(
            sGP.img,
            sGP.x_0,
            sGP.y_0,
            sGP.w+1,
            sGP.w+1,
            sGP.lineColor
        );

    //Draw bg
    drawRect(
            sGP.img,
            sGP.x_0+sGP.widthBorder,
            sGP.y_0+sGP.widthBorder,
            sGP.w-2*sGP.widthBorder-1,
            sGP.w-2*sGP.widthBorder-1,
            sGP.bgColor
        );


    //write digit
    if(sGP.depthLevel == 1){
        char chiffreStr[2];
        snprintf(chiffreStr, sizeof(chiffreStr), "%X", sGP.id);

        Uint8 r,g,b;
        SDL_GetRGB(sGP.digitColor, sGP.img->format, &r, &g, &b);

        SDL_Color textColor = {r, g, b, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(
            sGP.font,
            chiffreStr,
            textColor
        );

        if (!textSurface) {
            errx(1, "Error to create text surface.\n");
            return;
        }

        int textX = (sGP.x_0 + sGP.widthBorder +
                    (sGP.w - 2 * sGP.widthBorder) / 2) - textSurface->w / 2;

        int textY = (sGP.y_0 + sGP.widthBorder +
                    (sGP.w - 2 * sGP.widthBorder) / 2) - textSurface->h / 2;


        SDL_Rect dstRect = {textX, textY, textSurface->w, textSurface->h};

        SDL_BlitSurface(textSurface, NULL, sGP.img, &dstRect);
        SDL_FreeSurface(textSurface);
        return;
    }

    //start to create sub square / recurtion
    double new_widthBorder = sGP.widthBorder * gP.borderRatio / 2;

    double new_w = (sGP.w-2*sGP.widthBorder) / sGP.gS;


    //double new_w = (sGP.w - 2 * sP) / sGP.gS;
    sGP.depthLevel--;

    for (int x = 0; x < sGP.gS; x++)
    {
        for (int y = 0; y < sGP.gS; y++)
        {
            double new_x_0 = sGP.widthBorder + sGP.x_0 + x*new_w;

            double new_y_0 = sGP.widthBorder + sGP.y_0 + y*new_w;

            int new_id = sGP.id;

            if(sGP.depthLevel == 2){
                new_id = x + y * sGP.gS;
            }

            if(sGP.depthLevel == 1){
                //get digit to write in grid with diff color for solved digit

                new_id =  x + sGP.gS * y;
                int tempX = (int)((sGP.id%sGP.gS)*sGP.gS + x);
                int tempY = (int)((sGP.id/sGP.gS)*sGP.gS + y);

                if(defaultSG.grid[tempY]
                                [tempX] == -1){
                    int new_id = solvedSG.grid
                        [tempY]
                        [tempX];

                    if(sGP.gS == 4){
                        new_id--;
                    }

                    SubGridPara tempGP = sGP;
                    tempGP.x_0 = new_x_0;
                    tempGP.y_0 = new_y_0;
                    tempGP.w = new_w;
                    tempGP.widthBorder = new_widthBorder;
                    tempGP.id = new_id;

                    createGrid(tempGP, gP, defaultSG, solvedSG);

                    continue;

                }else{
                    new_id = defaultSG.grid
                        [tempY]
                        [tempX];

                    if(sGP.gS == 4){
                        new_id--;
                    }

                    SubGridPara tempGP = sGP;
                    tempGP.x_0 = new_x_0;
                    tempGP.y_0 = new_y_0;
                    tempGP.w = new_w;
                    tempGP.widthBorder = new_widthBorder;
                    tempGP.digitColor = sGP.lineColor;
                    tempGP.id = new_id;
                    tempGP.bgColor = sGP.bgColor2;


                    createGrid(tempGP, gP, defaultSG, solvedSG);
                    continue;
                }
            }

            SubGridPara tempGP = sGP;
            tempGP.x_0 = new_x_0;
            tempGP.y_0 = new_y_0;
            tempGP.w = new_w;
            tempGP.widthBorder = new_widthBorder;
            tempGP.id = new_id;

            createGrid(tempGP, gP, defaultSG, solvedSG);
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

void gridBuilder(SDL_Surface* img, TTF_Font* font, GridPara gP,
                    SudokuGrid defaultSG,
                    SudokuGrid solvedSG)
    {

    Uint32 bgColor = SDL_MapRGBA(img->format, 255, 255, 255, 0);

    //create background of the grid
    SDL_Rect rect =  {0, 0, gP.gridPxSize, gP.gridPxSize};
    SDL_FillRect(img, &rect, bgColor);

    //Color of the digit (default : red)
    Uint32 digitColor = SDL_MapRGBA(img->format, 255, 0, 0, 0);

    //light gray color
    Uint32 bgColor2 = SDL_MapRGBA(img->format, 225, 225, 225, 0);


    //start recursion of the grid build

    SubGridPara sGP = {
        .img = img,
        .x_0 = 0,
        .y_0 = 0,
        .w = img->w,
        .widthBorder = gP.widthBorder,
        .depthLevel = 3,
        .digitColor = digitColor,
        .id = 0,
        .lineColor = SDL_MapRGBA(img->format, 0, 0, 0, 0),
        .bgColor = bgColor,
        .font = font,
        .gS = defaultSG.gS == 9 ? 3 : 4,
        .bgColor2 = bgColor2
    };


    createGrid(sGP, gP, defaultSG, solvedSG);
}


/***************************************************************
 *  Function gridBuilder:
 *
 *  main function to create all the final grid
 *
 *  @output :
 *      - img (SDL_Surface*) : final img of the final grid
***************************************************************/
SDL_Surface* createOutputGrid(SudokuGrid defaultSG,
                                SudokuGrid solvedSG, GridPara gP){

    //Init TTF
    if (TTF_Init() < 0) {
        errx(EXIT_FAILURE ,"⚠️ Error to init TTF. ⚠️");
    }


    //Load font to write digit
    TTF_Font* font = TTF_OpenFont(PATH_FONT, gP.gridPxSize/defaultSG.gS
                                    * gP.fontRatio);
    if (!font) {
        errx(EXIT_FAILURE, "⚠️ Error to load font.ttf. ⚠️");
    }


    //Create output surface
    SDL_Surface* outputImg = SDL_CreateRGBSurfaceWithFormat(
        0, gP.gridPxSize,
        gP.gridPxSize, 32,
        SDL_PIXELFORMAT_ABGR8888
    );


    //build representation grid of sudokuGrid
    gridBuilder(outputImg, font, gP, defaultSG, solvedSG);



    //Close libs
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return outputImg;
}
