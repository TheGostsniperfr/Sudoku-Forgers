#include "SDL_Function/sdlFunction.h"
#include "Gaussian_Filter/gaussianFilter.h"
#include "GrayScale_Filter/grayScaleFilter.h"
#include "Histogram_Lib/histogram.h"
#include "Otsu_method/otsu.h"


int main(){
    

    SDL_Surface* img = loadImg("../data/sudoku_default_images/image_02.jpeg");
    
    if(img == NULL){
        printf("Error to load img !\n");
    }else{

        img = applyGrayScaleFilter(img);
        saveImg(img, "GrayScaleOutput.jpg");

        img = applyGaussianFilter(img);
        saveImg(img, "GaussianOutput.jpg");
        
        

        double* histo;
        histo = findHistogram(img);

        SDL_Surface* histoImg = createHistogramImg(histo);
        saveImg(histoImg, "HistoGram.jpg");

        double threshold = findOtsuSigma(histo);

        img = imgToBin(img, threshold);
        saveImg(img, "Binarized.jpg");

        free(histo);        
    }    

    
    SDL_FreeSurface(img);
}