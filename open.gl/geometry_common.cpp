#include "geometry_common.h"

#include "common.h"

int geometry_main(const GLchar * geo_shader_src) {
    GLFWwindow * window = init();
    const char * VERTEX_SHADER_SRC = GLSL(
        in vec2 pos;
        void main() {
            gl_Position = vec4(pos, 0.0, 1.0);
        }
    );
    const char * FRAG_SHADER_SRC = GLSL(
        out vec4 outColor;
        void main() {
            outColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    );
    GLuint vertex_shader = create_shader(VERTEX_SHADER_SRC, GL_VERTEX_SHADER);
    GLuint frag_shader = create_shader(FRAG_SHADER_SRC, GL_FRAGMENT_SHADER);
    GLuint geo_shader = create_shader(geo_shader_src, GL_GEOMETRY_SHADER);
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);
    glAttachShader(shader_program, geo_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    const float POINTS[] = {
        -0.45f,  0.45f,
         0.45f,  0.45f,
         0.45f, -0.45f,
        -0.45f, -0.45f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(POINTS), POINTS, GL_STATIC_DRAW);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLint pos_attr = glGetAttribLocation(shader_program, "pos");
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_POINTS, 0, 4);
    }
}
