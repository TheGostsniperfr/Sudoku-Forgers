#include "preProcessing/SDL_Function/sdlFunction.h"
#include "GUI/handleUtils.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <err.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"


#define DATASET_PATH "../../data/train-images.idx3-ubyte"
#define LABEL_PATH "../../data/train-labels.idx1-ubyte"
#define MAGIC_NUMBER 2051

SDL_Surface* getImageFromMnist(int index, Flag* flags) {

    FILE *file = fopen(DATASET_PATH, "rb");

    if (file == NULL) {
        perror("Can't open dataset file");
        exit(EXIT_FAILURE);
    }

    uint32_t magicNumber;

    // Read magic number
    fread(&magicNumber, sizeof(uint32_t), 1, file);
    magicNumber = ntohl(magicNumber);

    if (magicNumber != MAGIC_NUMBER) {
        fclose(file);
        errx
        (
            EXIT_FAILURE, 
            "Invalid magic number. \n"
            "Expected %u, got %u\n", 
            MAGIC_NUMBER, 
            magicNumber
        );

    }

    uint32_t nbImg, row, col;


    // Read number of images, rows, and columns
    fread(&nbImg, sizeof(uint32_t), 1, file);
    fread(&row, sizeof(uint32_t), 1, file);
    fread(&col, sizeof(uint32_t), 1, file);

    // Convert from big-endian to host byte order
    nbImg = ntohl(nbImg);
    row = ntohl(row);
    col = ntohl(col);

    if(flags[0].value == 1){
        printf("Number of images in the dataset: %u\n", nbImg);
        printf("Number of rows per image: %u\n", row);
        printf("Number of columns per image: %u\n", col); 
    }

    if((uint32_t) index >= nbImg){
        fclose(file);
        errx(EXIT_FAILURE, "Index of image out of range.");
    }

    //Calcul the pos of the cursor at the nth images index
    long newCursorPos = sizeof(uint32_t) * 3 + 
                        index * row * col * sizeof(uint8_t);

    //Move the cursor
    if (fseek(file, newCursorPos, SEEK_SET) != 0) {
        fclose(file);
        errx(EXIT_FAILURE, "Error : impossible to move the cursor.");
    }
    
    SDL_Surface* img = SDL_CreateRGBSurfaceWithFormat
                        (
                            0,
                            row,
                            col,
                            32,
                            SDL_PIXELFORMAT_ABGR8888
                        );

    Uint32* pixels = img->pixels;

    uint8_t pxVal;

    for (uint32_t i = 0; i < row*col; i++)
    {
        fread(&pxVal, sizeof(uint8_t), 1, file);

        Uint32 newPx = SDL_MapRGBA(img->format, pxVal, pxVal, pxVal, 255);
        pixels[i] = newPx;
    }    

    fclose(file);
    return img;
}