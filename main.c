#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <glfw3.h>
#include "main.h"
#include <malloc.h>

char* readFile(const char* filename) {
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( filename , "rb" );
    if ( !fp ) {
        perror(filename);
        exit(1);
    }

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if ( !buffer ) 
    {
        fclose(fp);
        fputs("memory alloc fails",stderr);
        exit(1);
    }

    /* copy the file into the buffer */
    if ( 1!=fread( buffer , lSize, 1 , fp) ) 
    {
        fclose(fp);
        free(buffer);
        fputs("entire read fails",stderr);
        exit(1);
    }
    fclose(fp);
    return buffer;
}

GLuint loadShader(const char* shaderPath, GLenum shaderType) 
{
    GLuint shaderID = glCreateShader(shaderType);

    char *shaderCode = readFile(shaderPath);

    glShaderSource(shaderID, 1, (const char**)&shaderCode, NULL);
    glCompileShader(shaderID);

    // Check for compilation errors (replace with your error-checking method)
    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logSize;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
        char* logMessage = (char*)malloc(logSize);
        glGetShaderInfoLog(shaderID, logSize, NULL, logMessage);
        printf("Shader compilation error in %s: \n%s\n", shaderPath, logMessage);
        free(logMessage);
        exit(1);
        return 0;
    }

    free(shaderCode);
    return shaderID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    if (!glfwInit())
    {
        printf("Error: Failed to initialize GLFW\n");
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "Warehouse", NULL, NULL);
    if (!window)
    {
        printf("Error: Failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }    
    glViewport(0, 0, 1200, 800);
    
    // glsl tutorials

    int success;
    char infoLog[512];
    unsigned int vertexShader = loadShader("..\\vertexShader.vert", GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadShader("..\\fragmentShader.frag", GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR IN SHADER PROGRAM: LINKING_FAILED");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shaderProgram);


    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    

    glfwTerminate();
    return 0;
}
