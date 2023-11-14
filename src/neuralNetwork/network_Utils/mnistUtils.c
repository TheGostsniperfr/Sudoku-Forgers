#include "preProcessing/SDL_Function/sdlFunction.h"
#include "GUI/handleUtils.h"
#include "neuralNetwork/network_Utils/struct.h"

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
#define DATA_MAGIC_NUMBER 2051
#define LABEL_MAGIC_NUMBER 2049


void freeImgContainer(ImgContainer* imgContainer){
    SDL_FreeSurface(imgContainer->img);
    free(imgContainer);
}


void getMnistDataHeader(FILE* dataFile, uint32_t* nbImg, uint32_t* row, uint32_t* col){
    uint32_t magicNumber;

    // Read magic number
    fread(&magicNumber, sizeof(uint32_t), 1, dataFile);
    magicNumber = ntohl(magicNumber);

    if (magicNumber != DATA_MAGIC_NUMBER) {
        fclose(dataFile);
        errx
        (
            EXIT_FAILURE,
            "Invalid magic number. \n"
            "Expected %u, got %u\n",
            DATA_MAGIC_NUMBER,
            magicNumber
        );

    }

    // Read number of images, rows, and columns
    fread(nbImg, sizeof(uint32_t), 1, dataFile);
    fread(row, sizeof(uint32_t), 1, dataFile);
    fread(col, sizeof(uint32_t), 1, dataFile);

    // Convert from big-endian to host byte order
    *nbImg = ntohl(*nbImg);
    *row = ntohl(*row);
    *col = ntohl(*col);
}

void getMnistLabelHeader(FILE* labelFile, uint32_t* nbLabel){
    uint32_t magicNumber;

    // Read magic number
    fread(&magicNumber, sizeof(uint32_t), 1, labelFile);
    magicNumber = ntohl(magicNumber);

    if (magicNumber != LABEL_MAGIC_NUMBER) {
        fclose(labelFile);
        errx
        (
            EXIT_FAILURE,
            "Invalid magic number. \n"
            "Expected %u, got %u\n",
            LABEL_MAGIC_NUMBER,
            magicNumber
        );

    }

    // Read number of images, rows, and columns
    fread(nbLabel, sizeof(uint32_t), 1, labelFile);
    // Convert from big-endian to host byte order
    *nbLabel = ntohl(*nbLabel);
}

ImgContainer* getImageFromMnist(int index, Flag* flags) {

    //load data (digits) file
    FILE *dataFile = fopen(DATASET_PATH, "rb");

    if (dataFile == NULL) {
        errx(EXIT_FAILURE, "Can't open dataset dataFile");
    }

    uint32_t nbImg, row, col;
    getMnistDataHeader(dataFile, &nbImg, &row, &col);

    if(flags[0].value == 1){
        printf("Number of images in the dataset: %u\n", nbImg);
        printf("Number of rows per image: %u\n", row);
        printf("Number of columns per image: %u\n", col);
    }

    if((uint32_t) index >= nbImg){
        fclose(dataFile);
        errx(EXIT_FAILURE, "Index of image out of range.");
    }

    //load label dataset
    FILE* labelFile = fopen(LABEL_PATH, "rb");
    if (labelFile == NULL) {
        fclose(dataFile);
        fclose(labelFile);
        errx(EXIT_FAILURE, "Can't open dataset dataFile");
    }

    uint32_t nbLabels;
    getMnistLabelHeader(labelFile, &nbLabels);
    if(nbImg != nbLabels){
        fclose(dataFile);
        fclose(labelFile);
        errx(EXIT_FAILURE, "Error : number of images and labels not equal.");
    }

    ImgContainer* imgContainer = calloc(1, sizeof(ImgContainer));
    if(imgContainer == NULL){
        fclose(dataFile);
        fclose(labelFile);
        free(imgContainer);
        errx(EXIT_FAILURE, "Impossible to alloc memory.");
    }

    //Calcul the pos of the cursor at the nth images index
    long newDataCursorPos = sizeof(uint32_t) * 4 +
                        index * row * col * sizeof(uint8_t);

    long newLabelCursorPos = sizeof(uint32_t) * 2 + index;

    //Move the cursors
    if ((fseek(dataFile, newDataCursorPos, SEEK_SET) != 0) |
            (fseek(labelFile, newLabelCursorPos, SEEK_SET) != 0)) {
        fclose(dataFile);
        fclose(labelFile);
        free(imgContainer);
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
    uint8_t labelVal = 0;
    fread(&labelVal, sizeof(uint8_t), 1, labelFile);


    for (uint32_t i = 0; i < row*col; i++)
    {
        fread(&pxVal, sizeof(uint8_t), 1, dataFile);

        Uint32 newPx = SDL_MapRGBA(img->format, pxVal, pxVal, pxVal, 255);
        pixels[i] = newPx;
    }

    fclose(dataFile);
    fclose(labelFile);

    imgContainer->img = img;
    imgContainer->label = labelVal;

    return imgContainer;
}


ImgContainer** getDataSet(int batchSize, Flag* flags){
    FILE* dataFile = fopen(DATASET_PATH, "rb");
    FILE* labelFile = fopen(LABEL_PATH, "rb");
    if((dataFile == NULL) | (labelFile == NULL)){
        errx(EXIT_FAILURE, "Can't open dataset dataFile");
    }

    uint32_t nbImg, row, col, nbLabels;
    getMnistDataHeader(dataFile, &nbImg, &row, &col);
    getMnistLabelHeader(labelFile, &nbLabels);

    if(nbImg != nbLabels){
        errx
        (
            EXIT_FAILURE, 
            "Number of img don't match with the number of labels"
        );
    }

    if(batchSize > (int)nbImg){
        if(flags[0].value == 1){
            printf("Warning : Batch size too high, setted on %d\n", nbImg);
        }
    }

    //Move the cursors
    if ((fseek(dataFile, 16, SEEK_SET) != 0) |
            (fseek(labelFile, 8, SEEK_SET) != 0)) {
        fclose(dataFile);
        fclose(labelFile);
        errx(EXIT_FAILURE, "Error : impossible to move the cursor.");
    }

    ImgContainer** globalImgContainer = calloc(nbImg, sizeof(ImgContainer*));

    for (int img_i = 0; img_i < (int)nbImg; img_i++)
    {

        ImgContainer* tmp = calloc(1, sizeof(ImgContainer));
        
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
        uint8_t labelVal = 0;
        fread(&labelVal, sizeof(uint8_t), 1, labelFile);


        for (uint32_t i = 0; i < row*col; i++)
        {
            fread(&pxVal, sizeof(uint8_t), 1, dataFile);

            Uint32 newPx = SDL_MapRGBA(img->format, pxVal, pxVal, pxVal, 255);
            pixels[i] = newPx;
        }

        

        tmp->img = img;
        tmp->label = labelVal;

        globalImgContainer[img_i] = tmp;
    }

    fclose(dataFile);
    fclose(labelFile);

    return globalImgContainer;
}