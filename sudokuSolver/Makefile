all:
	gcc -Wall -Wextra -c ../preProcessing/SDL_Function/sdlFunction.c `sdl2-config --cflags --libs` -lSDL2_image	-lm
	gcc -Wall -Wextra -c solver/sudoku_solver.c -lm
	gcc -Wall -Wextra -c convertLib/sudokuConvert.c -lm
	gcc -Wall -Wextra -c outputGrid/createOutputGrid.c `sdl2-config --cflags --libs` -lSDL2_image `sdl2-config --libs` -lSDL2_ttf -lm
	gcc -Wall -Wextra -o main main.c sdlFunction.o sudoku_solver.o createOutputGrid.o sudokuConvert.o `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf	-lm


clean:
	rm -f main
	rm -f *.
	rm -f *.o
	rm -f *.jpg
	rm -f *.result