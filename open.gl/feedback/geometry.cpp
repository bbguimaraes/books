#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/gl.h>

#include "common.h"

#define GLSL(src) "#version 150 core\n" # src

const GLchar * VERTEX_SHADER_SRC = GLSL(
    in float inValue;
    out float geoValue;
    void main() {
        geoValue = sqrt(inValue);
    }
);

const GLchar * GEOMETRY_SHADER_SRC = GLSL(
    layout(points) in;
    layout(triangle_strip, max_vertices = 3) out;
    in float[] geoValue;
    out float outValue;
    void main() {
        for(int i = 0; i < 3; ++i) {
            outValue = geoValue[0] + i;
            EmitVertex();
        }
        EndPrimitive();
    }
);

int main(int argc, char ** argv) {
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, false);
    GLFWwindow * window = create_window(800, 600, "OpenGL");
    glfwMakeContextCurrent(window);
    init_glew_or_exit();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SRC, nullptr);
    glCompileShader(vertex_shader);
    GLuint geo_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geo_shader, 1, &GEOMETRY_SHADER_SRC, nullptr);
    glCompileShader(geo_shader);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, geo_shader);
    const GLchar * feedback_varyings[] = {"outValue"};
    glTransformFeedbackVaryings(
        program, 1, feedback_varyings, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(program);
    glUseProgram(program);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLfloat data[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    GLint input_attr = glGetAttribLocation(program, "inValue");
    glEnableVertexAttribArray(input_attr);
    glVertexAttribPointer(input_attr, 1, GL_FLOAT, GL_FALSE, 0, 0);
    GLuint tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(data), nullptr, GL_STATIC_READ);
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);
    glBeginTransformFeedback(GL_TRIANGLES);
    glDrawArrays(GL_POINTS, 0, 5);
    glEndTransformFeedback();
    glFlush();
    GLfloat feedback[15];
    glGetBufferSubData(
        GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
    exit_on_gl_error();
    for(unsigned int i = 0; i < 15; ++i)
        std::printf("%f\n", feedback[i]);
    return 0;
}
