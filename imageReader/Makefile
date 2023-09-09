all:
	gcc `pkg-config --cflags gtk+-3.0` -Wall -Wextra -c -o imageReader.o imageReader.c `pkg-config --libs gtk+-3.0`

clean:
	rm -f imageReader
	rm -r *.o
