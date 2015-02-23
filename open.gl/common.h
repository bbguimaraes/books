#ifndef COMMON_H
#define COMMON_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

class GLFWwindow;

GLFWwindow * init();
GLFWwindow * create_window(int width, int height, const char * title);
void init_glew_or_exit();
void exit_on_gl_error();
GLuint create_vao();
GLuint create_vbo(size_t size, float vertices[]);
GLuint create_ebo(size_t size, unsigned int elements[]);
GLuint create_shader(const GLchar * src, GLenum type);
GLuint create_shader_or_exit(const GLchar * src, GLenum type);
void check_shader_or_exit(GLuint id);
bool is_valid_shader(GLuint id);
GLuint create_program(GLuint vertex_shader, GLuint frag_shader);

#endif
