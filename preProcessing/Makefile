all:
	gcc -Wall -Wextra -c preProcessing.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c gaussianFilter.c -lm `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -g -o main main.c preProcessing.o gaussianFilter.o -lm `sdl2-config --cflags --libs` -lSDL2_image

clean:
	rm -f preProcessing
	rm -f gaussianFilter
	rm -f main
	rm -f *.o
	rm -f *.jpeg
	rm -f *.jpg
	rm -f *.png
	

