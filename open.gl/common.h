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

#endif
