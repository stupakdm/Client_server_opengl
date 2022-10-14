CC=gcc
CPP=g++

CFLAGS = -c -Wall
GLFLAGS = -ldl -lglut -lglfw -lGLU -lGLEW -lGL -lpthread

all: game

game: glad.o init_game.o
	$(CPP) glad.o init_game.o -o init_game $(GLFLAGS)


init_game.o: init_game.cpp
	$(CPP) $(CFLAGS) init_game.cpp

glad.o:	glad.c
	$(CC) $(CFLAGS) glad.c

clean:
	rm *.o init_game
