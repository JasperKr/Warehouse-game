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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void inititalize_window(GLFWwindow** window, GLuint* shaderProgram) 
{
    if (!glfwInit())
    {
        printf("Error: Failed to initialize GLFW\n");
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    *window = glfwCreateWindow(1200, 800, "Warehouse", NULL, NULL);
    if (!*window)
    {
        printf("Error: Failed to create window\n");
        glfwTerminate();
    }
    glfwMakeContextCurrent(*window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
    }    
    glViewport(0, 0, 1200, 800);
    
    // glsl tutorials

    int success;
    char infoLog[512];
    unsigned int vertexShader = loadShader("..\\vertexShader.vert", GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadShader("..\\fragmentShader.frag", GL_FRAGMENT_SHADER);

    *shaderProgram = glCreateProgram();

    glAttachShader(*shaderProgram, vertexShader);
    glAttachShader(*shaderProgram, fragmentShader);
    glLinkProgram(*shaderProgram);

    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
        printf("ERROR IN SHADER PROGRAM: LINKING_FAILED");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

int main()
{
    stbi_set_flip_vertically_on_load(true);

    GLFWwindow* window;
    GLuint shaderProgram;
    inititalize_window(&window, &shaderProgram);

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // bind the Vertex Buffer Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  

    /* Texture stuff*/
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../assets/cow.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        printf("Loaded texture"); // does not print either
        glGenerateMipmap(GL_TEXTURE_2D);
        
    }
    else 
    {
        printf("Failed to load texture");
        exit(1);
    }

    stbi_image_free(data);

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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // render the quad
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteProgram(shaderProgram);
    

    glfwTerminate();
    return 0;
}
