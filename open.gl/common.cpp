#include "common.h"

#include <cstdlib>
#include <iostream>

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

GLuint create_vao() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

GLuint create_vbo(size_t size, float vertices[]) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return vbo;
}

GLuint create_ebo(size_t size, unsigned int elements[]) {
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, elements, GL_STATIC_DRAW);
    return ebo;
}

GLuint create_shader(const GLchar * src, GLenum type) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    return id;
}

void check_shader_or_exit(GLuint id) {
    if(!is_valid_shader(id)) {
        std::cerr
            << "glCompileShader(vertex_shader): failed, compile log follows"
            << std::endl;
        char buffer[512];
        glGetShaderInfoLog(id, 512, NULL, buffer);
        std::cerr << buffer;
        exit(1);
    }
}

GLuint create_shader_or_exit(const GLchar * src, GLenum type) {
    GLuint id = create_shader(src, type);
    check_shader_or_exit(id);
    return id;
}

bool is_valid_shader(GLuint id) {
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    return status == GL_TRUE;
}

GLuint create_program(GLuint vertex_shader, GLuint frag_shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, frag_shader);
    glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program);
    glUseProgram(program);
    return program;
}
