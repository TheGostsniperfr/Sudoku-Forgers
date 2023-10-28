all:
	gcc -Wall -Wextra -c ../preProcessing/SDL_Function/sdlFunction.c `sdl2-config --cflags --libs` -lSDL2_image	-lm
	gcc -Wall -Wextra -c sudokuSolver/sudoku_solver.c -lm
	gcc -Wall -Wextra -c aux/sudokuUtil.c `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf	-lm
	gcc -Wall -Wextra -c aux/sudokuHandle.c `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf	-lm
	gcc -Wall -Wextra -c outputGrid/createOutputGrid.c `sdl2-config --cflags --libs` -lSDL2_image `sdl2-config --libs` -lSDL2_ttf -lm
	gcc -Wall -Wextra -o solver solver.c sdlFunction.o sudoku_solver.o createOutputGrid.o sudokuUtil.o sudokuHandle.o `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf	-lm


clean:
	rm -f solver
	rm -f *.
	rm -f *.o
	rm -f *.jpg
	rm -f *.result