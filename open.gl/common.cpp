#include "common.h"

#include <cstdlib>
#include <iostream>

#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SQUARE_VERTICES_COUNT = 16;

const float SQUARE_VERTICES[] = {
    // pos        texcoords
    -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f,  0.5f, 1.0f, 0.0f,
     0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f,
};

const unsigned int SQUARE_ELEMENTS_COUNT = 6;

const GLuint SQUARE_ELEMENTS[] = {
    0, 1, 2,
    2, 3, 0,
};

const unsigned int CUBE_VERTICES_COUNT = 336;

const float CUBE_VERTICES[] = {
    // pos               color             texcoords
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

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

GLuint create_vbo(size_t size, const float vertices[]) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return vbo;
}

GLuint create_ebo(size_t size, const unsigned int elements[]) {
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

GLint create_texture_from_image(const char * filename, GLenum object) {
    GLuint tex;
    glActiveTexture(object);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
    unsigned char * image = SOIL_load_image(
        filename, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glGenerateMipmap(GL_TEXTURE_2D);
    return tex;
}

void setup_projection(GLuint program) {
    glm::mat4 view = glm::lookAt(
        glm::vec3(1.2f, 1.2f, 1.2f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f));
    GLint view_uni = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(view_uni, 1, GL_FALSE, glm::value_ptr(view));
    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);
    GLint proj_uni = glGetUniformLocation(program, "proj");
    glUniformMatrix4fv(proj_uni, 1, GL_FALSE, glm::value_ptr(proj));
}

float generate_angle(clock_t c, clock_t cs) {
    return static_cast<float>(c) / cs * 25.0f * glm::radians(180.0f);
}

void draw_scene(float angle, GLint model_uni, GLint ov_col_uni) {
    glm::mat4 model;
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(model_uni, 1, GL_FALSE, glm::value_ptr(model));
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    glClear(GL_STENCIL_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 36, 6);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDepthMask(GL_TRUE);
    model = glm::scale(
        glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::vec3(1.0f, 1.0f, -1.0f));
    glUniformMatrix4fv(model_uni, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(ov_col_uni, 0.3f, 0.3f, 0.3f);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glUniform3f(ov_col_uni, 1.0f, 1.0f, 1.0f);
    glDisable(GL_STENCIL_TEST);
}
