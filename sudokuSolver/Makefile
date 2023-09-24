all:
	gcc -Wall -Wextra -c ../preProcessing/SDL_Function/sdlFunction.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c solver/sudoku_solver.c
	gcc -Wall -Wextra -c outputGrid/createOutputGrid.c `sdl2-config --cflags --libs` -lSDL2_image `sdl2-config --libs` -lSDL2_ttf
	gcc -Wall -Wextra -o main main.c sdlFunction.o sudoku_solver.o createOutputGrid.o `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf	


clean:
	rm -f main
	rm -f *.
	rm -f *.o