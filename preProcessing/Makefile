all:
	gcc -Wall -Wextra -c SDL_Function/sdlFunction.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c Gaussian_Filter/gaussianFilter.c -lm `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c GrayScale_Filter/grayScaleFilter.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c Histogram_Lib/histogram.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c Otsu_method/otsu.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c Otsu_method/homo_transform.c `sdl2-config --cflags --libs` -lSDL2_image	



	gcc -Wall -Wextra -g -o main main.c sdlFunction.o gaussianFilter.o grayScaleFilter.o histogram.o otsu.o homo_transform.o -lm `sdl2-config --cflags --libs` -lSDL2_image

clean:
	rm -f sdlFunction
	rm -f gaussianFilter
	rm -f grayScaleFilter
	rm -f otsu
	rm -f homo_transform
	rm -f main
	rm -f *.o
	rm -f *.jpeg
	rm -f *.jpg
	rm -f *.png
	

