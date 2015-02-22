#include "common.h"

#include <cmath> // sin
#include <ctime> // clock, CLOCKS_PER_SEC

float vertices[] = {
     0.0f,  0.5f,
     0.5f, -0.5f,
    -0.5f, -0.5f,
};

const GLchar * vertex_shader_src =
    "#version 150 core\n"
    "\n"
    "in vec2 position;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = vec4(position, 0.0, 1.0);\n"
    "}\n";

const GLchar * frag_shader_src =
    "#version 150 core\n"
    "\n"
    "uniform vec3 triangleColor;\n"
    "out vec4 outColor;\n"
    "\n"
    "void main() {\n"
    "    outColor = vec4(triangleColor, 1.0);\n"
    "}\n";

int main() {
    GLFWwindow * window = init();
    create_vao();
    create_vbo(sizeof(vertices), vertices);
    GLuint vertex_shader = create_shader_or_exit(
        vertex_shader_src, GL_VERTEX_SHADER);
    GLuint frag_shader = create_shader_or_exit(
        frag_shader_src, GL_FRAGMENT_SHADER);
    GLuint program = create_program(vertex_shader, frag_shader);
    GLint pos_attr = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    GLint uni_color = glGetUniformLocation(program, "triangleColor");
    glUniform3f(uni_color, 1.0f, 0.0f, 0.0f);
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        float time = static_cast<float>(std::clock())
            / static_cast<float>(CLOCKS_PER_SEC) * 50.0f;
        glUniform3f(
            uni_color, (std::sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        exit_on_gl_error();
    }
    glfwTerminate();
    return 0;
}
