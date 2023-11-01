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

	gcc -Wall -Wextra -c Rotation/spin_rotate.c `sdl2-config --cflags --libs` -lSDL2_image -lm
	gcc -Wall -Wextra -c Bilinear_Interpolation/bilinear_interpolation.c `sdl2-config --cflags --libs` -lSDL2_image -lm
	gcc -Wall -Wextra -c Division9/division_by_9.c `sdl2-config --cflags --libs` -lSDL2_image -lm

	gcc -Wall -Wextra -g -o main main.c sdlFunction.o gaussianFilter.o grayScaleFilter.o histogram.o otsu.o morphology.o FindCorners.o rotation.o pixel.o homography.o mat.o spin_rotate.o bilinear_interpolation.o division_by_9.o -lm `sdl2-config --cflags --libs` -lSDL2_image

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
	rm -f spin_rotate
	rm -f bilinear_interpolation
	rm -f division_by_9
	rm -f main
	rm -f *.o
	rm -f *.jpeg
	rm -f *.jpg
	rm -f *.png
	

