#include "GUI/handleUtils.h"
#include "preProcessing/binarization_method/adaptiveThreshold.h"
#include "preProcessing/binarization_method/otsu.h"
#include "preProcessing/Gaussian_Filter/gaussianFilter.h"
#include "preProcessing/GrayScale_Filter/grayScaleFilter.h"
#include "preProcessing/Histogram_Lib/histogram.h"
#include "preProcessing/Morphology/morphology.h"
#include "preProcessing/contrast_Filter/contrastFilter.h"
#include "preProcessing/Illuminatio_Filter/IlluminatioFilter.h"
#include "preProcessing/median_Filter/medianFilter.h"

#include "preProcessing/preProcessingAux/preProcessingHandle.h"

#include "preProcessing/Rotate/spin_rotate.h"
#include "preProcessing/Corner_Finder/find_corners.h"
#include "preProcessing/Homography/homography.h"
#include "preProcessing/BlobDetection/BlobDetection.h"
#include "preProcessing/CaseDetection/case_detection.h"
#include "preProcessing/Noise_detection/Noise_detection.h"


int handleAllSteps(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* inputImgPath,
        Flag* flags)
    {
    /*
        Usage :
            [-all] -> do all the steps of preProcessing
    */

    if(flags[1].value == 1){
        printf(
            "################################################\n\n"
            "       🚀 Starting pre-processing.\n\n"
            "################################################\n\n"
        );

    }

    SDL_Surface* img = loadImg(inputImgPath);

    if(img == NULL){
        errx(EXIT_FAILURE, "Error to load img !");
    }else{

        if(flags[1].value == 1){
            printf("🚀 Starting to apply gray-scale filter.\n");
        }

        img = applyGrayScaleFilter(img);

        if(flags[1].value == 1){
            printf("✅ Success to apply gray-scale filter.\n");
        }

        if(flags[0].value == 1){
            saveImg(img, "GrayScaleOutput.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save GrayScaleOutput.jpg\n");
            }
        }

        if(flags[1].value == 1){
            printf("🚀 Starting to apply contrast filter.\n");
        }

        img = applyContrastFilter(img);

        if(flags[1].value == 1){
            printf("✅ Success to apply contrast filter.\n");
        }

        if(flags[0].value == 1){
            saveImg(img, "ContrastOutput.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save ContrastOutput.jpg\n");
            }
        }

        if(flags[1].value == 1){
            printf("🚀 Starting to apply illumination filter.\n");
        }

        img = applyIlluminationFilter(img);

        if(flags[1].value == 1){
            printf("✅ Success to apply illumination filter.\n");
        }

        if(flags[0].value == 1){
            saveImg(img, "IlluminationOutput.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save IlluminationOutput.jpg\n");
            }
        }

        if(flags[1].value == 1){
            printf("🚀 Starting to apply median filter.\n");
        }

        img = applyMedianFilter(img);

        if(flags[1].value == 1){
            printf("✅ Success to apply median filter.\n");
        }

        if(flags[0].value == 1){
            saveImg(img, "MedianOutput.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save MedianOutput.jpg\n");
            }
        }

        if(flags[1].value == 1){
            printf("🚀 Starting to apply gaussian filter.\n");
        }
        img = applyGaussianFilter(img);

        if(flags[1].value == 1){
            printf("✅ Success to apply gaussian filter.\n");
        }

        if(flags[0].value == 1){
            saveImg(img, "GaussianOutput.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save GaussianOutput.jpg\n");
            }

        }/*
        //Otsu method (old global method)
        if(flags[1].value == 1){
            printf("🚀 Starting to calcul histogram.\n");
        }
        double* histo;
        histo = findHistogram(img);

        SDL_Surface* histoImg = createHistogramImg(histo);
        if(flags[0].value == 1){
            saveImg(histoImg, "HistoGram.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save HistoGram.jpg\n");
            }
        }
        free(histoImg);

        if(flags[1].value == 1){
            printf("✅ Success to calcul histogram.\n");
        }

        if(flags[1].value == 1){
            printf("🚀 Starting image binarization.\n");
        }
        double threshold = findOtsuThreshold(histo, img->w*img->h);

        img = otsuBinarization(img, threshold);

        if(flags[1].value == 1){
            printf("✅ Success of image binarization.\n");
        }

        if(flags[0].value == 1){
            saveImg(img, "Binarized.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save Binarized.jpg\n");
            }
        }
        */

        if(flags[1].value == 1){
            printf("🚀 Starting to apply adaptive threshold.\n");
        }
        img = applyAdaptiveThreshold(img);

        if(flags[1].value == 1){
            printf("✅ Success to apply adaptive threshold.\n");
        }

        if(flags[0].value == 1){
            saveImg(img, "Binarized.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save Binarized.jpg\n");
            }

        }

        if(flags[1].value == 1){
            printf("🚀 Starting to apply morphology filter.\n");
        }

        //dilate
        img = applyMorphology(img, 0);
        //erode
        img = applyMorphology(img, 1);

        if(flags[1].value == 1){
            printf("✅ Success to apply morphology.\n");
        }

        if(flags[0].value == 1){
            saveImg(img, "Morphology.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save Morphology.jpg\n");
            }
        }

        saveImg(img, "Result.jpg");

        if(flags[1].value == 1){
            printf("💾 Success to save Result.jpg\n");
        }

        int size_blob = 0;
        SDL_Surface *blob = img;
        blob = Blob(blob, &size_blob);
        if(flags[1].value == 1){
            printf("✅ Success to apply blob detection.\n");
        }

        if (flags[0].value == 1){
            saveImg(blob, "Blob.jpg");
        }
        if (flags[1].value == 1){
            printf("💾 Success to save Blob.jpg\n");
        }

        if(flags[2].value == 1)
        {
            //Only for the fourth image
            int p[] = {
                413, //upper left corner -> x
                164, //upper left corner -> y
                424, // lower left corner -> x
                1309, // lower left corner -> y
                1539, // lower right corner -> x
                1305, // lower right corner -> y
                1546, // upper right corner -> x
                182, // upper right corner -> y
            };

            img = Homography_Transform(img, 1000, p);
            if(flags[1].value == 1){
                printf("✅ Success to apply homography.\n");
            }

            if(flags[0].value == 1){
                saveImg(img, "Homography.jpg");
                if(flags[1].value == 1){
                    printf("💾 Success to save Homogaphy.jpg\n");
                }
            }

            //GridCell* Cases = CaseDetection(img);
            //Cases = NoiseDetection(Cases);

        }

        /*
        SDL_Surface* grid = Blob(img, (int*)0);
        int* points = FindCoins(grid);

        img = Homography_Transform(img, 1000, points);
        if(flags[1].value == 1){
            printf("✅ Success to apply homography.\n");
        }

        if(flags[0].value == 1){
            saveImg(img, "Homography.jpg");
            if(flags[1].value == 1){
                printf("💾 Success to save Homogaphy.jpg\n");
            }
        }

        GridCell* Cases = CaseDetection(img);
        //Cases = noise_detetcion(Cases);
        */

        /* SDL_Surface* homographic_img = Homography_Transform(img, 1000);
        saveImg(homographic_img, "Homographic_img.jpg");  */


        //free(histo);

        //save final image



        if(flags[1].value == 1){
            printf(
                "\n################################################\n\n"
                "       ✅ Success to pre-processing image.\n\n"
                "################################################\n"
            );
        }
    }


    SDL_FreeSurface(img);

    return EXIT_SUCCESS;
}

int handleTurnNDegree(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* inputImgPath,
        Flag* flags __attribute__((unused)))
    {
    /*
        Usage :
            [-r] <number of degree> -> turn the image
    */

    Spin_rotate(loadImg(inputImgPath));

    if(argc != 1){
        errx(EXIT_FAILURE, ERROR_NB_ARG);
    }

    int degree = atoi(argv[0]);

    //Rotate Img
    //Save img

    if(flags[1].value == 1){
        printf("✅ Success to rotate the image by %d°\n", degree);
    }

    return EXIT_SUCCESS;
}

int handlePrintHelp(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* inputImgPath __attribute__((unused)),
        Flag* flags __attribute__((unused)))
    {
    /*
        Usage :
            [--help] -> print the help
    */

   printf
   (
        "Usage : preProcessing [IMAGE DIR] [OPTIONS]\n\n"
        "-all           ->      Do all the steps\n"
        "-r             ->      Rotate the image\n"
        "-save          ->      Save all steps images\n"
        "-verbose       ->      Print verbose\n"
        "--help         ->      Show the help panel\n"
    );

    return EXIT_SUCCESS;
}