#include <stdio.h>
#include <stdlib.h>
#include <glad.h>

unsigned int shader_newShader(const char* filepath, int shaderType)
{
    // Open file
    FILE * fptr = fopen(filepath, "rb");

    // Determine filesize
    fseek(fptr, 0L, SEEK_END);
    long fileSize = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    // Allocate memory of appropriate size
    char* newShaderSource = malloc(sizeof(char) * fileSize + 1);

    // Read contents of file into character buffer
    fread(newShaderSource, 1, fileSize, fptr);

    // For debugging purposes
    //printf("File contents:\n%s\n\n", vertexShaderSource);

    // Create and compile shader
    unsigned int newShader;
    newShader = glCreateShader(shaderType);

    glShaderSource(newShader, 1, (const char**)&newShaderSource, NULL);
    glCompileShader(newShader);

    // Final cleanup
    fclose(fptr);
    free(newShaderSource);
    

    return newShader;
}