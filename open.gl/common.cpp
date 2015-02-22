#include "common.h"

#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

GLFWwindow * init() {
    glfwInit();
    GLFWwindow * window = create_window(800, 600, "OpenGL");
    glfwMakeContextCurrent(window);
    init_glew_or_exit();
    return window;
}

GLFWwindow * create_window(int width, int height, const char * title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void init_glew_or_exit() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        std::cerr << "glewInit: failed" << std::endl;
        std::exit(1);
    }
    // "GLEW has a problem with core contexts. It calls
    // glGetString(GL_EXTENSIONS), which causes GL_INVALID_ENUM on GL 3.2+ core
    // context as soon as glewInit() is called.
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glGetError();
}

void exit_on_gl_error() {
    GLenum error;
    if((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "glGetError: " << gluErrorString(error) << std::endl;
        std::exit(1);
    }
}
