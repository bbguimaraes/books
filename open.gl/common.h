#ifndef COMMON_H
#define COMMON_H

#include <ctime> // clock_t

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

class GLFWwindow;

extern const unsigned int SQUARE_VERTICES_COUNT;
extern const float SQUARE_VERTICES[];
extern const unsigned int SQUARE_ELEMENTS_COUNT;
extern const GLuint SQUARE_ELEMENTS[];

extern const unsigned int CUBE_VERTICES_COUNT;
extern const float CUBE_VERTICES[];

GLFWwindow * init();
GLFWwindow * create_window(int width, int height, const char * title);
void init_glew_or_exit();
void exit_on_gl_error();
GLuint create_vao();
GLuint create_vbo(size_t size, const float vertices[]);
GLuint create_ebo(size_t size, const unsigned int elements[]);
GLuint create_shader(const GLchar * src, GLenum type);
GLuint create_shader_or_exit(const GLchar * src, GLenum type);
void check_shader_or_exit(GLuint id);
bool is_valid_shader(GLuint id);
GLuint create_program(GLuint vertex_shader, GLuint frag_shader);
GLint create_texture_from_image(const char * filename, GLenum object);
void setup_projection(GLuint program);
float generate_angle(clock_t c, clock_t cs);
void draw_scene(float angle, GLint model_uni, GLint ov_col_uni);

#endif
