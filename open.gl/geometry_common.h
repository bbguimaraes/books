#ifndef GEOMETRY_COMMON_H
#define GEOMETRY_COMMON_H

#include <GL/glew.h>
#include <GL/gl.h>

#define GLSL(src) "#version 150 core\n" # src

int geometry_main(const GLchar * geo_shader_src);

#endif
