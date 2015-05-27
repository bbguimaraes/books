#include "common.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLSL(src) "#version 150 core\n" # src

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const GLchar * VERTEX_SHADER_SRC = GLSL(
    in vec2 pos;
    void main() {
        gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    }
);

const char * FRAG_SHADER_SRC = GLSL(
    out vec4 outColor;
    void main() {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
);

const GLchar * FEEDBACK_SHADER_SRC = GLSL(
    in vec2 pos;
    out vec2 outValue;
    uniform vec2 g;
    const float step = 0.01;
    void main() {
        outValue = pos + step * normalize(g - pos);
    }
);

int main(int argc, char ** argv) {
    GLfloat POINTS_INITIAL[] = {
        // pos
        -0.5f,  0.5f,
         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,
    };
    unsigned int POINTS_COUNT = sizeof(POINTS_INITIAL) / sizeof(GLfloat);
    GLfloat points[POINTS_COUNT];
    std::memcpy(points, POINTS_INITIAL, POINTS_COUNT);
    GLFWwindow * window = init();
    GLuint feedback_shader = create_shader_or_exit(
        FEEDBACK_SHADER_SRC, GL_VERTEX_SHADER);
    GLuint feedback_program = glCreateProgram();
    glAttachShader(feedback_program, feedback_shader);
    const GLchar * feedback_varyings[] = {"outValue"};
    glTransformFeedbackVaryings(
        feedback_program, 1, feedback_varyings, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(feedback_program);
    glUseProgram(feedback_program);
    GLuint feedback_vao;
    glGenVertexArrays(1, &feedback_vao);
    glBindVertexArray(feedback_vao);
    GLint pos_attr = glGetAttribLocation(feedback_program, "pos");
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(
        pos_attr, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    GLuint g_uni = glGetUniformLocation(feedback_program, "g");
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // TODO GL_STATIC_READ?
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(POINTS_INITIAL), POINTS_INITIAL, GL_STATIC_READ);
    GLuint vertex_shader = create_shader(VERTEX_SHADER_SRC, GL_VERTEX_SHADER);
    GLuint frag_shader = create_shader(FRAG_SHADER_SRC, GL_FRAGMENT_SHADER);
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    pos_attr = glGetAttribLocation(shader_program, "pos");
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(
        pos_attr, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnable(GL_DEPTH_TEST);
    glPointSize(5.0f);
    while(!glfwWindowShouldClose(window)) {
        exit_on_gl_error();
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDisable(GL_RASTERIZER_DISCARD);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_POINTS, 0, 4);
        glUseProgram(feedback_program);
        glBindVertexArray(feedback_vao);
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if((0 <= x && x <= 800) && (0 <= y && y <= 600)) {
            x = (x - WINDOW_WIDTH  / 2.0) / (WINDOW_WIDTH  / 2.0);
            y = (y - WINDOW_HEIGHT / 2.0) / (WINDOW_HEIGHT / 2.0);
            y = -y;
            glUniform2f(g_uni, x, y);
        }
        glEnable(GL_RASTERIZER_DISCARD);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo);
        glBeginTransformFeedback(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, 5);
        glEndTransformFeedback();
        glFlush();
    }
}
