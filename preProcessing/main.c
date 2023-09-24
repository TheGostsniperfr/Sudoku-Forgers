#include "SDL_Function/sdlFunction.h"
#include "Gaussian_Filter/gaussianFilter.h"
#include "GrayScale_Filter/grayScaleFilter.h"
#include "Histogram_Lib/histogram.h"
#include "Otsu_method/otsu.h"
#include "Image_Manipulation/rotation.h"


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
        free(histoImg);        

        double threshold = findOtsuThreshold(histo, img->w*img->h);

        img = binarization(img, threshold);
        saveImg(img, "Binarized.jpg");

        SDL_Surface* rotated_img = Rotated_image(img, 10);
        saveImg(rotated_img, "Rotated_img.jpg");

        free(histo);        
    }    

    
    SDL_FreeSurface(img);
}