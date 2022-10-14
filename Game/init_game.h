#ifndef INIT_GAME_H
#define INIT_GAME_H
#include <cmath>
//#include "glad/glad.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include "GLFW/glfw3.h"
#include "stb_image.h"
#define LINUX_DATA 100

class init_socket
{
private:
    int data_socket, socket_linux;;
public:
    char buffer[100];
    init_socket();

    int data_send(const char data[]);
    int data_recv();
    int end_socket();
};

class init_game
{
private:
    GLFWwindow* window1;
    GLuint shaderProgram1;
    GLuint VBO, VAO;

public:
    init_game();
    void display();

    void init_cube();
    int play_game(init_socket sock);
};


#endif // INIT_GAME_H
