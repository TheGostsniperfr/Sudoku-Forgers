#include "preProcessing.h"
#include "gaussianFilter.h"

int main(){
    

    SDL_Surface* img = loadImg("../data/sudoku_default_images/image_01.jpeg");
    if(img == NULL){
        printf("Error to load img !\n");
    }else{
        img = applyGaussianFilter(img);
        printf("Apply Gaussian filter\n");
        saveImg(img, "test.jpg");
    }


    

    
    SDL_FreeSurface(img);


}