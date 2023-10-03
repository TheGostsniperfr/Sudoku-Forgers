#include "SDL_Function/sdlFunction.h"
#include "Gaussian_Filter/gaussianFilter.h"
#include "GrayScale_Filter/grayScaleFilter.h"
#include "Histogram_Lib/histogram.h"
#include "Otsu_method/otsu.h"
#include "Rotation/rotation.h"
#include "Morphology/morphology.h"


/*
#include "Homography/homography.h"
*/


int main(){


    SDL_Surface* img = loadImg("../data/sudoku_default_images/image_01.jpeg");
    
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

        img = applyMorphology(img, 0);
        img = applyMorphology(img, 1);

        saveImg(img, "Morphology.jpg");

        /* double angle;
        angle = Find_angle_to_Rotate(img);
        printf("%lf\n", angle); */

        //SDL_Surface* rotated_img = Rotated_image(img, -45);
        //saveImg(rotated_img, "Rotated_img.jpg");

        /* 
        SDL_Surface* homographic_img = Homography_Transform(img, 100);
        saveImg(homographic_img, "Homographic_img.jpg"); 
        */

        free(histo);        
    }    

    
    SDL_FreeSurface(img);
}