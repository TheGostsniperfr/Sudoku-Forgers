all:
	gcc -Wall -Wextra -c SDL_Function/sdlFunction.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c Gaussian_Filter/gaussianFilter.c -lm `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c GrayScale_Filter/grayScaleFilter.c `sdl2-config --cflags --libs` -lSDL2_image	

	gcc -Wall -Wextra -g -o main main.c sdlFunction.o gaussianFilter.o grayScaleFilter.o -lm `sdl2-config --cflags --libs` -lSDL2_image

clean:
	rm -f sdlFunction
	rm -f gaussianFilter
	rm -f grayScaleFilter
	rm -f main
	rm -f *.o
	rm -f *.jpeg
	rm -f *.jpg
	rm -f *.png
	

