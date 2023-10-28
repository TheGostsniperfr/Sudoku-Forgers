all:
	gcc -Wall -Wextra -c SDL_Function/sdlFunction.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c Gaussian_Filter/gaussianFilter.c -lm `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c GrayScale_Filter/grayScaleFilter.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c Histogram_Lib/histogram.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c Otsu_method/otsu.c `sdl2-config --cflags --libs` -lSDL2_image
	gcc -Wall -Wextra -c Morphology/morphology.c `sdl2-config --cflags --libs` -lSDL2_image	
	
	gcc -Wall -Wextra -c Pixel/pixel.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c Corner_Finder/FindCorners.c `sdl2-config --cflags --libs` -lSDL2_image		
	gcc -Wall -Wextra -c Rotation/rotation.c `sdl2-config --cflags --libs` -lSDL2_image -lm
	gcc -Wall -Wextra -c Matrix/mat.c `sdl2-config --cflags --libs` -lSDL2_image -lm
	gcc -Wall -Wextra -c Homography/homography.c `sdl2-config --cflags --libs` -lSDL2_image -lm

	gcc -Wall -Wextra -g -o main main.c sdlFunction.o gaussianFilter.o grayScaleFilter.o histogram.o otsu.o morphology.o FindCorners.o rotation.o pixel.o homography.o mat.o -lm `sdl2-config --cflags --libs` -lSDL2_image

clean:
	rm -f sdlFunction
	rm -f gaussianFilter
	rm -f grayScaleFilter
	rm -f otsu
	rm -f morphology
	rm -f homography
	rm -f coins_detecter
	rm -f rotation
	rm -f pixel
	rm -f mat
	rm -f main
	rm -f *.o
	rm -f *.jpeg
	rm -f *.jpg
	rm -f *.png
	

