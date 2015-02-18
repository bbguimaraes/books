#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow * create_window(int width, int height, const char * title);
void init_glew();
void loop(GLFWwindow * window);
void check_gl_error();

int main() {
    glfwInit();
    GLFWwindow * window = create_window(800, 600, "OpenGL");
    glfwMakeContextCurrent(window);
    init_glew();
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    std::cout << vertex_buffer << std::endl;
    loop(window);
    glfwTerminate();
    return 0;
}

GLFWwindow * create_window(int width, int height, const char * title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void init_glew() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        std::cerr << "glewInit: failed" << std::endl;
        exit(1);
    }
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glGetError();
}

void loop(GLFWwindow * window) {
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        check_gl_error();
    }
}

void check_gl_error() {
    GLenum error;
    if((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "glGetError: " << gluErrorString(error) << std::endl;
        exit(1);
    }
}
