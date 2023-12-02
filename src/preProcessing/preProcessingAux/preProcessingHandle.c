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
#include "preProcessing/Image_Clean/Image_Clean.h"

#include "neuralNetwork/network_Utils/struct.h"

/***************************************************************
 *  Function handleAllSteps :
 *
 *  Handle function to process all the step of image treatment
 *
 *  @input :
 *      - argc (int) : number of args
 *      - argv[] (char*) : list of arg
 *      - inputImgPath (char*) : path of the input image
 *      - flags (Flag*) : flag of the program
 *
 *  @output :
 *      - (int) : state of the program
***************************************************************/

void* handleAllSteps(
		int argc __attribute__((unused)),
		char* argv[] __attribute__((unused)),
		char* inputImgPath,
		Flag* flags)
{
	/*
		Usage :
			[-all] -> do all the steps of preProcessing
	*/

	if(flags[0].value == 1){
		printf(
			"################################################\n\n"
			"       🚀 Starting pre-processing.\n\n"
			"################################################\n\n"
		);

	}

	SDL_Surface* img = loadImg(inputImgPath);


	AllStepResult* allStepResult = calloc(1, sizeof(AllStepResult));

	if(img == NULL){
		errx(EXIT_FAILURE, "Error to load img !");
	}


	if(flags[0].value == 1){
		printf("🚀 Starting to apply gray-scale filter.\n");
	}

	img = applyGrayScaleFilter(img);

	if(flags[0].value == 1){
		printf("✅ Success to apply gray-scale filter.\n");
	}

	if(flags[1].value == 1){
		saveImg(img, "GrayScaleOutput.jpg");
		if(flags[0].value == 1){
			printf("💾 Success to save GrayScaleOutput.jpg\n");
		}
	}

	if(flags[0].value == 1){
		printf("🚀 Starting to apply contrast filter.\n");
	}

	img = applyContrastFilter(img);

	if(flags[0].value == 1){
		printf("✅ Success to apply contrast filter.\n");
	}

	if(flags[1].value == 1){
		saveImg(img, "ContrastOutput.jpg");
		if(flags[0].value == 1){
			printf("💾 Success to save ContrastOutput.jpg\n");
		}
	}

	if(flags[0].value == 1){
		printf("🚀 Starting to apply illumination filter.\n");
	}

	img = applyIlluminationFilter(img);

	if(flags[0].value == 1){
		printf("✅ Success to apply illumination filter.\n");
	}

	if(flags[1].value == 1){
		saveImg(img, "IlluminationOutput.jpg");
		if(flags[0].value == 1){
			printf("💾 Success to save IlluminationOutput.jpg\n");
		}
	}

	if(flags[0].value == 1){
		printf("🚀 Starting to apply median filter.\n");
	}

	img = applyMedianFilter(img);

	if(flags[0].value == 1){
		printf("✅ Success to apply median filter.\n");
	}

	if(flags[1].value == 1){
		saveImg(img, "MedianOutput.jpg");
		if(flags[0].value == 1){
			printf("💾 Success to save MedianOutput.jpg\n");
		}
	}

	if(flags[0].value == 1){
		printf("🚀 Starting to apply gaussian filter.\n");
	}
	img = applyGaussianFilter(img);

	if(flags[0].value == 1){
		printf("✅ Success to apply gaussian filter.\n");
	}

	if(flags[1].value == 1){
		saveImg(img, "GaussianOutput.jpg");
		if(flags[0].value == 1){
			printf("💾 Success to save GaussianOutput.jpg\n");
		}

	}

	if(flags[0].value == 1){
		printf("🚀 Starting to apply adaptive threshold.\n");
	}
	img = applyAdaptiveThreshold(img);

	if(flags[0].value == 1){
		printf("✅ Success to apply adaptive threshold.\n");
	}

	if(flags[1].value == 1){
		saveImg(img, "Binarized.jpg");
		if(flags[0].value == 1){
			printf("💾 Success to save Binarized.jpg\n");
		}

	}

	if(flags[0].value == 1){
		printf("🚀 Starting to apply morphology filter.\n");
	}

	//dilate
	img = applyMorphology(img, 0);
	//erode
	img = applyMorphology(img, 1);

	if(flags[0].value == 1){
		printf("✅ Success to apply morphology.\n");
	}

	if(flags[1].value == 1){
		saveImg(img, "Morphology.jpg");
		if(flags[0].value == 1){
			printf("💾 Success to save Morphology.jpg\n");
		}
	}
	allStepResult->binarizedImg = img;

	saveImg(img, "Result.jpg");

	if(flags[0].value == 1){
		printf("💾 Success to save Result.jpg\n");
	}

	if(flags[0].value == 1){
		printf("🚀 Starting to apply Blob.\n");
	}
	
	int size_blob = 0;
	SDL_Surface* blob = Blob(img, &size_blob);
	int* points = FindCoins(blob);
	Pointx_y p1;
	Pointx_y p2;
	Pointx_y p3;
	Pointx_y p4;
	SDL_Surface* img_bis;

	p1.x = points[0];
	p1.y = points[1];

	p2.x = points[6];
	p2.y = points[7];

	p3.x = points[4];
	p3.y = points[5];

	p4.x = points[2];
	p4.y = points[3];

	while(!isSquare_Blob(p1, p2, p3, p4))
	{
		size_blob = 0;
		img_bis = Remove_Blob(img, blob);
		blob = Blob(img_bis, &size_blob);
		points = FindCoins(blob);

		//ReSet of each coordonates
		p1.x = points[0];
		p1.y = points[1];

		p2.x = points[6];
		p2.y = points[7];

		p3.x = points[4];
		p3.y = points[5];

		p4.x = points[2];
		p4.y = points[3];
	}
	free(img_bis);

	if(flags[0].value == 1){
		printf("✅ Success to apply blob detection.\n");
	}

	allStepResult->gridImg = SDL_ConvertSurfaceFormat
								(
									blob,
									SDL_PIXELFORMAT_ABGR8888,
									0
								);
	SDL_BlitSurface(allStepResult->gridImg, NULL, blob, NULL);

	if (flags[1].value == 1){
		saveImg(blob, "Blob.jpg");
		if (flags[0].value == 1){
			printf("💾 Success to save Blob.jpg\n");
		}
	}
	

	if(flags[0].value == 1){
		printf("🚀 Starting to apply Homography_Transform.\n");
	}
	img = Homography_Transform(img, 1000, points);

	allStepResult->homographyImg = SDL_ConvertSurfaceFormat
								(
									img,
									SDL_PIXELFORMAT_ABGR8888,
									0
								);
	SDL_BlitSurface(allStepResult->homographyImg, NULL, img, NULL);

	if(flags[0].value == 1){
		printf("✅ Success to apply homography.\n");
	}

	if(flags[1].value == 1){
		saveImg(img, "Homography.jpg");
		if(flags[0].value == 1){
			printf("💾 Success to save Homogaphy.jpg\n");
		}
	}



	GridCell* Cases = CaseDetection(img);
	for (int i = 0; i < 81; i++)
	{
		char buffer[1024];
		snprintf(buffer, sizeof(buffer), "post_%d.jpg", i);
		saveImg(Cases[i].image, buffer);
	}
	Image_Clean(Cases);

	for (int i = 0; i < 81; i++)
	{
		char buffer[1024];
		snprintf(buffer, sizeof(buffer), "after_%d.jpg", i);
		saveImg(Cases[i].image, buffer);
	}


	//save final image



	if(flags[0].value == 1){
		printf(
			"\n################################################\n\n"
			"       ✅ Success to pre-processing image.\n\n"
			"################################################\n"
		);
	}

	allStepResult->gridCells = Cases;


	SDL_FreeSurface(img);

	return (void*)allStepResult;
}



/***************************************************************
 *  Function handleTurnNDegree :
 *
 *  Handle function to show the manual rotating interface
 *
 *  @input :
 *      - argc (int) : number of args
 *      - argv[] (char*) : list of arg
 *      - inputImgPath (char*) : path of the input image
 *      - flags (Flag*) : flag of the program
 *
 *  @output :
 *      - (int) : state of the program
***************************************************************/

void* handleTurnNDegree(
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

	if(flags[0].value == 1){
		printf("✅ Success to rotate the image by %d°\n", degree);
	}

	return EXIT_SUCCESS;
}


/***************************************************************
 *  Function handlePrintHelp :
 *
 *  Handle function to print the help menu
 *
 *  @input :
 *      - argc (int) : number of args
 *      - argv[] (char*) : list of arg
 *      - inputImgPath (char*) : path of the input image
 *      - flags (Flag*) : flag of the program
 *
 *  @output :
 *      - (int) : state of the program
***************************************************************/

void* handlePrintPreProHelp(
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