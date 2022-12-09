CC=gcc
CPP=g++

CFLAGS = -c -Wall
GLFLAGS = -ldl -lglut -lglfw -lGLU -lGLEW -lGL -lpthread

all: game

game: glad init_game.o common_compile
	$(CPP) *.o -o init_game $(GLFLAGS)

camera: glad stb
	$(CPP) stb_image.o glad.o camera.cpp $(GLFLAGS) -o camera

another_camera: glad stb
	$(CPP) stb_image.o glad.o another_camera.cpp $(GLFLAGS) -o another_camera

stb: stb_image.cpp
	$(CPP) $(CFLAGS) stb_image.cpp

init_game.o: init_game.cpp
	$(CPP) $(CFLAGS) init_game.cpp

glad:	glad.c
	$(CC) $(CFLAGS) glad.c

common_compile:
	$(CPP) $(CFLAGS) $(GLFLAGS) common/*.cpp

clean:
	rm *.o init_game

