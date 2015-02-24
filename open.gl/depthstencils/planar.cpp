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
    "in vec3 position;\n"
    "in vec3 color;\n"
    "in vec2 texcoord;\n"
    "\n"
    "out vec3 Color;\n"
    "out vec2 Texcoord;\n"
    "\n"
    "uniform vec3 overrideColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "\n"
    "void main() {\n"
    "    Color = overrideColor * color;\n"
    "    Texcoord = texcoord;\n"
    "    gl_Position = proj * view * model * vec4(position, 1.0);\n"
    "}\n";

const GLchar * frag_shader_src =
    "#version 150 core\n"
    "\n"
    "in vec3 Color;\n"
    "in vec2 Texcoord;\n"
    "\n"
    "out vec4 outColor;\n"
    "\n"
    "uniform sampler2D texKitten;\n"
    "uniform sampler2D texPuppy;\n"
    "\n"
    "void main() {\n"
    "    vec4 texColor = mix(\n"
    "        texture(texKitten, Texcoord),\n"
    "        texture(texPuppy, Texcoord),\n"
    "        0.5);\n"
    "    outColor = vec4(Color, 1.0) * texColor;\n"
    "}\n";

int main() {
    GLFWwindow * window = init();
    create_vao();
    create_vbo(CUBE_VERTICES_COUNT * sizeof(float), CUBE_VERTICES);
    GLuint vertex_shader = create_shader_or_exit(
        vertex_shader_src, GL_VERTEX_SHADER);
    GLuint frag_shader = create_shader_or_exit(
        frag_shader_src, GL_FRAGMENT_SHADER);
    GLuint program = create_program(vertex_shader, frag_shader);
    GLint pos_attr = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(
        pos_attr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    GLint color_attr = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(color_attr);
    glVertexAttribPointer(
        color_attr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        reinterpret_cast<void *>(3 * sizeof(float)));
    GLint tex_attr = glGetAttribLocation(program, "texcoord");
    glEnableVertexAttribArray(tex_attr);
    glVertexAttribPointer(
        tex_attr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
        reinterpret_cast<void *>(6 * sizeof(float)));
    GLint tex = create_texture_from_image("../sample.png", GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "texKitten"), 0);
    tex = create_texture_from_image("../sample2.png", GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(program, "texPuppy"), 1);
    GLint ov_col_uni = glGetUniformLocation(program, "overrideColor");
    setup_projection(program);
    GLint model_uni = glGetUniformLocation(program, "model");
    exit_on_gl_error();
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        float angle = generate_angle(std::clock(), CLOCKS_PER_SEC);
        draw_scene(angle, model_uni, ov_col_uni);
        exit_on_gl_error();
    }
    glfwTerminate();
    return 0;
}
