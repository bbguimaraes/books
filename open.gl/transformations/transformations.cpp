#include "common.h"
#include "transformations_common.h"

#include <ctime> // clock, CLOCKS_PER_SEC

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

const GLchar * vertex_shader_src =
    "#version 150 core\n"
    "\n"
    "in vec2 position;\n"
    "in vec2 texcoord;\n"
    "\n"
    "out vec2 Texcoord;\n"
    "\n"
    "uniform mat4 trans;\n"
    "\n"
    "void main() {\n"
    "    Texcoord = texcoord;\n"
    "    gl_Position = trans * vec4(position, 0.0, 1.0);\n"
    "}\n";

int main() {
    GLFWwindow * window = init();
    create_vao();
    create_vbo(
        SQUARE_VERTICES_COUNT * sizeof(SQUARE_VERTICES[0]),
        SQUARE_VERTICES);
    create_ebo(
        SQUARE_ELEMENTS_COUNT * sizeof(SQUARE_ELEMENTS[0]),
        SQUARE_ELEMENTS);
    GLuint vertex_shader = create_shader_or_exit(
        vertex_shader_src, GL_VERTEX_SHADER);
    GLuint frag_shader = create_shader_or_exit(
        FRAG_SHADER_SRC, GL_FRAGMENT_SHADER);
    GLuint program = create_program(vertex_shader, frag_shader);
    setup_shader_attrs(program);
    GLint trans_uni = glGetUniformLocation(program, "trans");
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        float angle = static_cast<float>(std::clock())
            / static_cast<float>(CLOCKS_PER_SEC)
            * 25.0f * 3.1415f;
        glm::mat4 trans;
        trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(trans_uni, 1, GL_FALSE, glm::value_ptr(trans));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        exit_on_gl_error();
    }
    glfwTerminate();
    return 0;
}
