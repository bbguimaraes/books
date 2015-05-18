#ifndef TRANSFORMATION_COMMON_O
#define TRANSFORMATION_COMMON_O

#include <GL/glew.h>
#include <GL/gl.h>

extern const GLchar * FRAMEBUFFERS_VERTEX_SHADER;
extern const GLchar * FRAMEBUFFERS_FRAG_SHADER;
extern const GLchar * FRAMEBUFFERS_VERTEX_SHADER_2D;
extern const unsigned int VERTICES_COUNT;
extern const float VERTICES[];

void setup_shader_attrs(GLuint program);
void create_frame_buffer_or_exit(
    unsigned int width, unsigned int height, GLuint * id, GLuint *tex);
void create_frame_buffer(
    unsigned int width, unsigned int height, GLuint * id, GLuint *tex);
bool check_frame_buffer(GLuint frame_buffer);
int framebuffers_main(const GLchar * frag_shader_src_2d);

#endif
