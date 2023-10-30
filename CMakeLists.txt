cmake_minimum_required(VERSION 3.20)
project(Warehouse)

include_directories("deps/include")

find_library(GLFW_PATH glfw3 "deps/libs")
#add_library(glfwLib STATIC "${GLFW}")
link_libraries("${GLFW_PATH}")

add_executable(Warehouse main.c)
target_sources(Warehouse PUBLIC main.c PUBLIC glad.c)
