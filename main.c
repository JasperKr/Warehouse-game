#include <stdio.h>
#include "glad.h"
#define GLFW_INCLUDE_NONE
#include "glfw3.h"

int main() {

    if (glfwInit()) {
        printf("Init: Success");
    } else {
        printf("Init: Fail");
    }

    glfwTerminate();
}

