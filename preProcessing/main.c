#include "SDL_Function/sdlFunction.h"
#include "Gaussian_Filter/gaussianFilter.h"
#include "GrayScale_Filter/grayScaleFilter.h"
#include "Histogram_Lib/histogram.h"


int main(){
    

    SDL_Surface* img = loadImg("../data/sudoku_default_images/image_02.jpeg");
    
    if(img == NULL){
        printf("Error to load img !\n");
    }else{
        img = applyGaussianFilter(img);
        printf("Apply Gaussian filter\n");
        saveImg(img, "GaussianOutput.jpg");
        
        img = applyGrayScaleFilter(img);
        saveImg(img, "GrayScaleOutput.jpg");

        int* histo;
        histo = findHistogram(img);

        printf()

        //SDL_Surface* histoImg = createHistogramImg(histo);
        //saveImg(histoImg, "HistoGram.jpg");

        //free(histo);        
    }    

    
    SDL_FreeSurface(img);


}