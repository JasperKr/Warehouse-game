#include <stdio.h>
#include <stdbool.h>
#include <glad.h>
#define GLFW_INCLUDE_NONE
#include <glfw3.h>
#include "main.h"

int main() {


    // Initialize GLFW
    if (glfwInit()) 
    {
        printf("GLFW Init: Success\n");
    } else
    {
        printf("GLFW Init: Fail\n");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Warehouse", NULL, NULL);
    if (window == NULL)
    {
        printf("Window: Failed to create window\n");
        glfwTerminate();
        return -1;
    } else
    {
        printf("Window: Creation successful\n");
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("GLAD init: Fail\n");
    } else
    {
        printf("GLAD init: Success\n");
    }

    // Set viewport and sets callback for resizing the window
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_VERTEX_ARRAY, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const char *vertexShaderSource =    
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int succes;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succes);
    if (!succes)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    // Main window loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Clean up GLFW and exit with code 0 (SUCCES)
    glfwTerminate();
    return 0;
}


// Method definitions
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}