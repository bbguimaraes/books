#include "common.h"
#include "transformations_common.h"

#include <ctime> // CLOCKS_PER_SEC

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
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "\n"
    "void main() {\n"
    "    Texcoord = texcoord;\n"
    "    gl_Position = proj * view * model * vec4(position, 0.0, 1.0);\n"
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
    glm::mat4 view = glm::lookAt(
        glm::vec3(1.2f, 1.2f, 1.2f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f));
    GLint view_uni = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(view_uni, 1, GL_FALSE, glm::value_ptr(view));
    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);
    GLint proj_uni = glGetUniformLocation(program, "proj");
    glUniformMatrix4fv(proj_uni, 1, GL_FALSE, glm::value_ptr(proj));
    GLint model_uni = glGetUniformLocation(program, "model");
    float angle = 3.1415f * 0.5f;
    const float INITIAL_V = 3.1415f * 0.1f;
    float v = 0.0f;
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            v = INITIAL_V;
        angle += v;
        v *= 1.0f - 0.25f / static_cast<float>(CLOCKS_PER_SEC) * 10e4;
        glm::mat4 model;
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(model_uni, 1, GL_FALSE, glm::value_ptr(model));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        exit_on_gl_error();
    }
    glfwTerminate();
    return 0;
}
