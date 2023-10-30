cmake_minimum_required(VERSION 3.20)
project(Warehouse)
add_executable(Warehouse main.c)

include_directories("deps/include")

find_library(GLFW_PATH glfw3 "deps/libs")
#add_library(glfwLib STATIC "${GLFW}")
target_link_libraries(Warehouse PUBLIC "${GLFW_PATH}")