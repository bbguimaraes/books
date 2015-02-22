#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "common.h"

void loop(GLFWwindow * window);

int main() {
    GLFWwindow * window = init();
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    std::cout << vertex_buffer << std::endl;
    loop(window);
    glfwTerminate();
    return 0;
}

void loop(GLFWwindow * window) {
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        exit_on_gl_error();
    }
}
