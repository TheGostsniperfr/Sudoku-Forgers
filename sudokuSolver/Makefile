all:
	gcc -Wall -Wextra -c ../preProcessing/SDL_Function/sdlFunction.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -c solver/sudoku_solver.c
	gcc -Wall -Wextra -c outputGrid/createOutputGrid.c `sdl2-config --cflags --libs` -lSDL2_image	
	gcc -Wall -Wextra -o main main.c sdlFunction.o sudoku_solver.o createOutputGrid.o `sdl2-config --cflags --libs` -lSDL2_image	


clean:
	rm -f main
	rm -f *.
	rm -f *.o