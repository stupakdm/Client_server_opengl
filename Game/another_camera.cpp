#include <glad/glad.h>
#include <vector>

#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "camera_functions.h"
#include "shader_m.h"
#include <iostream>

std::vector<float> vertices_coord;
std::vector<float> coords_left;
std::vector<float> coords_right;
std::vector<float> coords_up;
std::vector<float> coords_down;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

float xPos, yPos;

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;
int pressed = -1;
float start_time = 0.0f;
float angle = 0.0f;
float prev_angle = 0.0f;

glm::vec3 rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  -3.0f)
};

glm::vec3 rightarrow_Position[] = {
    glm::vec3(0.5f, 0.0f, -1.0f)
};

glm::vec3 leftarrow_Position[] = {
    glm::vec3(-0.5f, 0.0f, -1.0f)
};

glm::vec3 uparrow_Position[] = {
    glm::vec3(0.0f, 0.35f, -1.0f)
};

glm::vec3 downarrow_Position[] = {
    glm::vec3(0.0f, -0.35f, -1.0f)
};



void drawLoop(GLFWwindow *window, Shader cube, Shader arrows, unsigned int VAO[], unsigned int texture1, unsigned int texture2, bool is_fov=false)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // camera/view transformation
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    // activate shader
    cube.use();

    // bind textures on corresponding texture units

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    cube.setMat4("projection", projection);

    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    cube.setMat4("view", view);

    // render boxes
    glBindVertexArray(VAO[0]);
    float step = 0.1f;
    for (unsigned int i = 0; i < 1; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        //float angle = 1.0f;
        if ((start_time == 0.0f) && (pressed != -1))
        {

            start_time = glfwGetTime();
            prev_angle = angle;
        }
        if (pressed == 1)   //right
        {
            //next_time = glfwGetTime() - start_time;
            rotation_axis = glm::vec3(0.0f, 1.0f, 0.0f);
            angle = angle + step;
            if (abs(angle - prev_angle) >= 90.0f)
            {
                pressed = -1;
                start_time = 0.0f;
            }
        }
        if (pressed == 2)   //left
        {
            //next_time = glfwGetTime() - start_time;
            rotation_axis = glm::vec3(0.0f, 1.0f, 0.0f);
            angle = angle - step;
            if (abs(angle - prev_angle) >= 90.0f)
            {
                pressed = -1;
                start_time = 0.0f;
            }
        }

        if (pressed == 3)   //up
        {
            //next_time = glfwGetTime() - start_time;
            rotation_axis = glm::vec3(1.0f, 0.0f, 0.0f);
            angle = angle - step;
            if (abs(angle - prev_angle) >= 90.0f)
            {
                pressed = -1;
                start_time = 0.0f;
            }
        }

        if (pressed == 4)   //down
        {
            //next_time = glfwGetTime() - start_time;
            rotation_axis = glm::vec3(1.0f, 0.0f, 0.0f);
            angle = angle + step;
            if (abs(angle - prev_angle) >= 90.0f)
            {
                pressed = -1;
                start_time = 0.0f;
            }
        }

        //if (pressed != -1)
            //std::cout << glm::radians(angle) << std::endl;
        model = glm::rotate(model, glm::radians(angle)/*(float)glfwGetTime()glm::radians(angle)*/, rotation_axis);



        /*}

        if (pressed == 1)
        {
            model = glm::rotate(model, glm::radians(angle)/*(float)glfwGetTime()glm::radians(angle), rotation_axis);
        */
        cube.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    if (!is_fov)
    {
        //RIGHT
        arrows.use();
        arrows.setMat4("projection", projection);
        arrows.setMat4("view", view);

        glBindVertexArray(VAO[1]);
        if (pressed == 1)
        {
            arrows.setBool("gray", true);
        }
        else
        {
            arrows.setBool("gray", false);
        }
        for (unsigned int i=0;i<1;i++)
        {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, rightarrow_Position[i]);
            float angle = 0.0f;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            arrows.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 12);
        }


        //LEFT
        arrows.use();
        arrows.setMat4("projection", projection);
        arrows.setMat4("view", view);

        glBindVertexArray(VAO[2]);
        if (pressed == 2)
        {
            arrows.setBool("gray", true);
        }
        else
        {
            arrows.setBool("gray", false);
        }
        for (unsigned int i=0;i<1;i++)
        {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, leftarrow_Position[i]);
            float angle = 0.0f;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            arrows.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 12);
        }


        //UP
        arrows.use();
        arrows.setMat4("projection", projection);
        arrows.setMat4("view", view);

        glBindVertexArray(VAO[3]);
        if (pressed == 3)
        {
            arrows.setBool("gray", true);
        }
        else
        {
            arrows.setBool("gray", false);
        }
        for (unsigned int i=0;i<1;i++)
        {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, uparrow_Position[i]);
            float angle = 0.0f;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            arrows.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 12);
        }


        //DOWN
        arrows.use();
        arrows.setMat4("projection", projection);
        arrows.setMat4("view", view);

        glBindVertexArray(VAO[4]);
        if (pressed == 4)
        {
            arrows.setBool("gray", true);
        }
        else
        {
            arrows.setBool("gray", false);
        }
        for (unsigned int i=0;i<1;i++)
        {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, downarrow_Position[i]);
            float angle = 0.0f;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            arrows.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 12);
        }

    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------

    //Camera_functions functions(Shader our("shaders/camera_shaders/camera_vertex.glsl", "shaders/camera_shaders/camera_fragment.glsl"), Shader now("shaders/camera_shaders/camera_vertex.glsl", "shaders/camera_shaders/camera_fragment.glsl"));

    Shader ourShader_cubes("shaders/camera_shaders/camera_vertex.glsl", "shaders/camera_shaders/camera_fragment.glsl");

    Shader ourShader_arrows("shaders/arrow_shaders/arrow_vertex.glsl", "shaders/arrow_shaders/arrow_fragment.glsl");

    //Camera_functions functions(ourShader_cubes, ourShader_arrows);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // world space positions of our cubes
    /*glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  -1.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/

    float vertices_right_arrow[] = {
        -0.25f, -0.5f, 0.0f, 0.0f, 0.0f,        //A
         0.0f,  -0.5f, 0.0f, 1.0f, 0.0f,        //B
         0.25f,  0.0f, 0.0f, 1.0f, 1.0f,        //D
         0.25f,  0.0f, 0.0f, 1.0f, 1.0f,        //D
         0.0f,   0.0f, 0.0f, 0.0f, 1.0f,        //C
        -0.25f, -0.5f, 0.0f, 0.0f, 0.0f,        //A

        -0.25f,  0.5f, 0.0f, 0.0f, 0.0f,        //A
         0.0f,   0.5f, 0.0f, 1.0f, 0.0f,        //B
         0.25f,  0.0f, 0.0f, 1.0f, 1.0f,        //D
         0.25f,  0.0f, 0.0f, 1.0f, 1.0f,        //D
         0.0f,   0.0f, 0.0f, 0.0f, 1.0f,        //C
        -0.25f,  0.5f, 0.0f, 0.0f, 0.0f,        //A


    };

    float vertices_left_arrow[] = {
         0.25f, -0.5f, 0.0f, 0.0f, 0.0f,        //A
         0.0f,  -0.5f, 0.0f, 1.0f, 0.0f,        //B
        -0.25f,  0.0f, 0.0f, 1.0f, 1.0f,        //D
        -0.25f,  0.0f, 0.0f, 1.0f, 1.0f,        //D
         0.0f,   0.0f, 0.0f, 0.0f, 1.0f,        //C
         0.25f, -0.5f, 0.0f, 0.0f, 0.0f,        //A

         0.25f,  0.5f, 0.0f, 0.0f, 0.0f,        //A
         0.0f,   0.5f, 0.0f, 1.0f, 0.0f,        //B
        -0.25f,  0.0f, 0.0f, 1.0f, 1.0f,        //D
        -0.25f,  0.0f, 0.0f, 1.0f, 1.0f,        //D
         0.0f,   0.0f, 0.0f, 0.0f, 1.0f,        //C
         0.25f,  0.5f, 0.0f, 0.0f, 0.0f,        //A


    };

    float vertices_up_arrow[] =
    {
        -0.5f,-0.25f, 0.0f, 0.0f, 0.0f,
         0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.25f, 0.0f, 1.0f, 1.0f,
         0.0f, 0.25f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,-0.25f, 0.0f, 0.0f, 0.0f,

         0.5f,-0.25f, 0.0f, 0.0f, 0.0f,
         0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.25f, 0.0f, 1.0f, 1.0f,
         0.0f, 0.25f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
         0.5f,-0.25f, 0.0f, 0.0f, 0.0f,

    };

    float vertices_down_arrow[] =
    {
        -0.5f, 0.25f, 0.0f, 0.0f, 0.0f,
         0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,-0.25f, 0.0f, 1.0f, 1.0f,
         0.0f,-0.25f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.25f, 0.0f, 0.0f, 0.0f,

         0.5f, 0.25f, 0.0f, 0.0f, 0.0f,
         0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,-0.25f, 0.0f, 1.0f, 1.0f,
         0.0f,-0.25f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, 0.25f, 0.0f, 0.0f, 0.0f,

    };

    for (int i=0;i<12;i++)
    {
        vertices_left_arrow[i*5] /= 5;
        vertices_left_arrow[i*5+1] /= 5;
        vertices_left_arrow[i*5+2] /= 5;

        vertices_right_arrow[i*5] /= 5;
        vertices_right_arrow[i*5+1] /= 5;
        vertices_right_arrow[i*5+2] /= 5;

        vertices_up_arrow[i*5] /= 5;
        vertices_up_arrow[i*5+1] /= 5;
        vertices_up_arrow[i*5+2] /= 5;

        vertices_down_arrow[i*5] /= 5;
        vertices_down_arrow[i*5+1] /= 5;
        vertices_down_arrow[i*5+2] /= 5;
    }




    /*float vertices_coord[] = {
       -0.25f, -0.5f,
        0.0f,  -0.5f,
        0.0f,   0.0f,

       -0.25f,  0.5f,
        0.0f,   0.5f,
        0.0f,   0.0f,

        0.25f, -0.5f,
        0.0f,  -0.5f,
        0.0f,   0.0f,

        0.25f,  0.5f,
        0.0f,   0.5f,
        0.0f,   0.0f,
    }*/
    /*vertices_coord.push_back(-0.25f);
    vertices_coord.push_back(0.5f);
    vertices_coord.push_back(0.0f);
    vertices_coord.push_back(-0.5f);
    vertices_coord.push_back(0.0f);
    vertices_coord.push_back(0.0f);

    vertices_coord.push_back(-0.25f);
    vertices_coord.push_back(0.5f);
    vertices_coord.push_back(0.0f);
    vertices_coord.push_back(0.5f);
    vertices_coord.push_back(0.0f);
    vertices_coord.push_back(0.0f);

    vertices_coord.push_back(0.25f);
    vertices_coord.push_back(-0.5f);
    vertices_coord.push_back(0.0f);
    vertices_coord.push_back(-0.5f);
    vertices_coord.push_back(0.0f);
    vertices_coord.push_back(0.0f);

    vertices_coord.push_back(0.25f);
    vertices_coord.push_back(0.5f);
    vertices_coord.push_back(0.0f);
    vertices_coord.push_back(0.5f);
    vertices_coord.push_back(0.0f);
    vertices_coord.push_back(0.0f);*/


    /*glm::vec3 rightarrow_Position[] = {
        glm::vec3(1.3f, 0.0f, 0.0f)
    };

    glm::vec3 leftarrow_Position[] = {
        glm::vec3(-1.3f, 0.0f, 0.0f)
    };*/

    /*for (int i=0;i<6;i++)
    {
        vertices_coord[i*2] += rightarrow_Position[0][0];
        vertices_coord[i*2+1] += rightarrow_Position[0][1];

    }
    for (int i=6;i<12;i++)
    {
        vertices_coord[i*2] += leftarrow_Position[0][0];
        vertices_coord[i*2+1] += leftarrow_Position[0][1];

    }*/

    coords_left.push_back(-0.8f);
    coords_left.push_back(-1.0f);
    coords_left.push_back(0.25f);
    coords_left.push_back(-0.25f);


    coords_right.push_back(1.0f);
    coords_right.push_back(0.8f);
    coords_right.push_back(0.25f);
    coords_right.push_back(-0.25f);


    coords_up.push_back(0.18f);
    coords_up.push_back(-0.18f);
    coords_up.push_back(-0.7f);
    coords_up.push_back(-0.95f);

    coords_down.push_back(0.18f);
    coords_down.push_back(-0.18f);
    coords_down.push_back(0.95f);
    coords_down.push_back(0.7f);


    unsigned int VBO[5], VAO[5];
    glGenVertexArrays(5, VAO);
    glGenBuffers(5, VBO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(std::string("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(std::string("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader_cubes.use();
    ourShader_cubes.setInt("texture1", 0);
    ourShader_cubes.setInt("texture2", 1);


    //Bind VBO, VAO for RIGHT arrow

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_right_arrow), vertices_right_arrow, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // tell opengl for each shader to use programm
    //ourShader_arrows.use();

    //Bind VBO, VAO for LEFT arrow

    glBindVertexArray(VAO[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_left_arrow), vertices_left_arrow, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // tell opengl for each shader to use programm
    //ourShader_arrows.use();


    //Bind VBO, VAO for UP arrow
    glBindVertexArray(VAO[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_up_arrow), vertices_up_arrow, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Bind VBO, VAO for DOWN arrow
    glBindVertexArray(VAO[4]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_down_arrow), vertices_down_arrow, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    ourShader_arrows.use();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        if (fov != 45.0f)
        {
            drawLoop(window, ourShader_cubes, ourShader_arrows, VAO, texture1, texture2, true);
            continue;
        }
        else
        {
            drawLoop(window, ourShader_cubes, ourShader_arrows, VAO, texture1, texture2, false);
        }

        /*

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // activate shader
        ourShader_cubes.use();

        // bind textures on corresponding texture units

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        ourShader_cubes.setMat4("projection", projection);

        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        ourShader_cubes.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO[0]);

        for (unsigned int i = 0; i < 1; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime()/*glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader_cubes.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ourShader_arrows.use();
        ourShader_arrows.setMat4("projection", projection);
        ourShader_arrows.setMat4("view", view);

        glBindVertexArray(VAO[1]);
        for (unsigned int i=0;i<1;i++)
        {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, rightarrow_Position[i]);
            float angle = 0.0f;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader_arrows.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 12);
        }

        ourShader_arrows.use();
        ourShader_arrows.setMat4("projection", projection);
        ourShader_arrows.setMat4("view", view);

        glBindVertexArray(VAO[2]);
        for (unsigned int i=0;i<1;i++)
        {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, leftarrow_Position[i]);
            float angle = 0.0f;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader_arrows.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 12);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();*/
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(3, VAO);
    glDeleteBuffers(3, VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;*/
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    //std::cout << xposIn << "  " << yposIn << std::endl;
    /*float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);*/
}

int check_point();

//glfw: click mouse
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS)
            {
                double x_pos, y_pos;
                glfwGetCursorPos(window, &x_pos, &y_pos);
                xPos = x_pos/SCR_WIDTH;
                yPos = y_pos/SCR_HEIGHT;
                xPos = (2.0*xPos)-1.0;
                yPos = (2.0*yPos)-1.0;
                if (fov == 45.0f)
                {
                    if (pressed == -1)
                        pressed = check_point();
                }
                std::cout << "pressed: " << pressed << '\n';
                std::cout << xPos << "   " << yPos << std::endl;
                std::cout << "MOUSE left input working\n";
                break;
            }
            else
            {
                if (action == GLFW_RELEASE)
                {
                    std::cout << "MOUSE left released\n";
                }
                break;
            }
    }
}

int check_point()
{
    /*std::cout << "left:";
    std::cout << coords_left[1] << "  " << coords_left[0] << ";";
    std::cout << coords_left[3] << "  " << coords_left[2] << std::endl;

    std::cout << "right:";
    std::cout << coords_right[1] << "  " << coords_right[0] << ";";
    std::cout << coords_right[3] << "  " << coords_right[2] << std::endl;*/
    if ((xPos >=coords_left[1]) && (xPos <=coords_left[0]) && (yPos >= coords_left[3]) && (yPos <= coords_left[2]))
    {
        return 2;
    }

    if ((xPos >=coords_right[1]) && (xPos <=coords_right[0]) && (yPos >= coords_right[3]) && (yPos <= coords_right[2]))
    {
        return 1;
    }

    if ((xPos >=coords_up[1]) && (xPos <=coords_up[0]) && (yPos >= coords_up[3]) && (yPos <= coords_up[2]))
    {
        return 3;
    }

    if ((xPos >=coords_down[1]) && (xPos <=coords_down[0]) && (yPos >= coords_down[3]) && (yPos <= coords_down[2]))
    {
        return 4;
    }
    /*for (int i =0;i<4;i++)
    {
        float xb = vertices_coord[i*3+2]-vertices_coord[i*3];
        float yb = vertices_coord[i*3+3]-vertices_coord[i*3+1];
        float xc = vertices_coord[i*3+4] - vertices_coord[i*3];
        float yc = vertices_coord[i*3+5]-vertices_coord[i*3+1];

        float xp = xPos - vertices_coord[i*3];
        float yp = yPos - vertices_coord[i*3+1];
        float d = xb*yc - yb*xc;
        if (d!=0)
        {
            float bb = xp*yc-xc*yp;
            float cc = xb*yp-xp*yb;
            if (d>0)
            {
                if ((bb>=0) && (cc>=0) && (bb<=d) && (cc<=d))
                {
                    if (i <2)
                        return 1;
                    else
                        return 2;

                }

            }
            if (d<=0)
            {
                if ((bb<=0) && (cc<=0) && (bb>=d) && (cc>=d))
                {
                    if (i<2)
                        return 1;
                    else
                        return 2;
                }
            }
        }

    }*/
    return -1;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

