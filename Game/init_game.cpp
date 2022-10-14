#include "init_game.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <unistd.h>
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <cstdio>

#define SOCKET_NAME "/tmp/S0cketGamePlAy4Life.socket"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position, 1.0);\n"
    "ourColor = color;\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "color = ourColor;\n"
    "}\n\0";

void error ( int error, const char * description )
{
    fputs ( description, stderr );
}

void key ( GLFWwindow * window, int key, int scancode, int action, int mods )
{
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose ( window, GL_TRUE );
}

/*
void init_game::display ()
{
    int width, height;

    glfwGetFramebufferSize ( window1, &width, &height );

    float   ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();
}
*/

init_game::init_game()
{
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    GLfloat vertices[] = {
            // Positions
             0.5f, -0.5f, 0.0f,  // Bottom Right
            -0.5f, -0.5f, 0.0f,  // Bottom Left
             0.0f,  0.5f, 0.0f   // Top
        };
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO

    shaderProgram1 = shaderProgram;
    window1 = window;
}

int init_socket::data_recv()
{
    memset(&buffer, 0, LINUX_DATA);
    read(data_socket, buffer, 1);
    printf("buffer2: %s\n", buffer);
    return 0;
}

int init_socket::data_send(const char *data)
{
    memset(&buffer, 0, LINUX_DATA);
    strcpy(buffer, data);
    buffer[LINUX_DATA-1] = 0;
    recv(data_socket, buffer, LINUX_DATA, 0);
    //write(data_socket, buffer, LINUX_DATA);
    return 0;
}

int init_socket::end_socket()
{
    unlink(SOCKET_NAME);
    close(socket_linux);
    while (wait(NULL) > 0);
    return 0;
}

int init_game::play_game(init_socket sock)
{
    char msg[LINUX_DATA];
    glViewport(0, 0, 800, 600);
    char i = 0x40;

    /*glfwMakeContextCurrent(window1);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window1, key);

    glfwSetFramebufferSizeCallback(window1, framebuffer_size_callback);*/
       //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLfloat red, green, blue;
    red = 0.0f;
    green = 0.0f;
    blue = 0.0f;
    while(!glfwWindowShouldClose(window1))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);

               // Update the uniform color

        if (strcmp(sock.buffer, "1") == 0)
        {
            red = 1.0f;
            green = 0.0f;
            blue = 0.0f;
        }
        if (strcmp(sock.buffer, "2") == 0)
        {
            red = 0.0f;
            green = 1.0f;
            blue = 0.0f;
        }
        if (strcmp(sock.buffer, "0") == 0)
        {
            break;
        }
        //GLfloat timeValue = glfwGetTime();
        //GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram1, "ourColor");
        glUniform4f(vertexColorLocation, red, green, blue, 1.0f);

               // Draw the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

               // Swap the screen buffers
        glfwSwapBuffers(window1);
        red = 1.0f;
        green = 1.0f;
        blue = 1.0f;
        //printf("PreRecv\n");
        sock.data_recv();
        printf("%s\n", sock.buffer);
        //printf("PostRecv\n");
        /*
        glUseProgra
        //display();
        glfwSwapBuffers(window1);
        glfwPollEvents();
        msg[0] = i;
        msg[1] = '\n';
        //printf("%c", i);
        if (i == 0x60)
        {
            //strcpy(msg, "exit");
            sock.data_send(msg);
            //break;
        }
        sock.data_send(msg);
        if (i<0x60)
            i++;*/
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

init_socket::init_socket()
{
    int err;
    struct sockaddr_un addr;
    int pid;
    char *my_argv[3] = {"../training", "create", NULL};

    unlink(SOCKET_NAME);

    socket_linux = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (socket_linux < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path)-1);       //Заполняем структуру для общения по сокетам

    err = bind(socket_linux, (const struct sockaddr*)&addr, sizeof(struct sockaddr_un));
    if (err == -1)
    {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    err = listen(socket_linux, 10);
    if (err == -1)
    {
        perror("listen error");
        exit(EXIT_FAILURE);
    }
    //buffer initialize
    memset(&buffer, 0, LINUX_DATA);
    for (int i =0;i<50;i++)
        buffer[i] = (char)(i+0x30);
    buffer[50] = 0;
    printf("PreAccepting\n");

    pid = fork();
    if (pid < 0)
    {
        printf("Error forking\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {

        err = execve(my_argv[0], (char**)my_argv, NULL);
        if (err == -1)
        {
            printf("Exec error\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }


    data_socket = accept(socket_linux, NULL, NULL);
    printf("accepting socket\n");



}



int main()
{

    init_game game;

    init_socket play_socket;

    game.play_game(play_socket);

    play_socket.end_socket();

    return 0;



}
