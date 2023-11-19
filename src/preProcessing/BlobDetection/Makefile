all:
	gcc -Wall -Wextra -c BlobDetection.c `sdl2-config --cflags --libs` -lSDL2_image
	gcc -Wall -Wextra -c ../SDL_Function/sdlFunction.c `sdl2-config --cflags --libs` -lSDL2_image

	gcc BlobDetection.o sdlFunction.o `sdl2-config --cflags --libs` -lSDL2_image -lm -o Blob

clean:
	rm -f Blob
	rm -f *.o