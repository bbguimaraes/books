#include "common.h"

#include <SOIL/SOIL.h>

float vertices[] = {
    // pos        texcoords
    -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f,  0.5f, 1.0f, 0.0f,
     0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f,
};

GLuint elements[] = {
    0, 1, 2,
    2, 3, 0,
};

const GLchar * vertex_shader_src =
    "#version 150 core\n"
    "\n"
    "in vec2 position;\n"
    "in vec2 texcoord;\n"
    "\n"
    "out vec2 Texcoord;\n"
    "\n"
    "void main() {\n"
    "    Texcoord = texcoord;\n"
    "    gl_Position = vec4(position, 0.0, 1.0);\n"
    "}\n";

const GLchar * frag_shader_src =
    "#version 150 core\n"
    "\n"
    "in vec2 Texcoord;\n"
    "\n"
    "out vec4 outColor;\n"
    "\n"
    "uniform sampler2D texKitten;\n"
    "uniform sampler2D texPuppy;\n"
    "\n"
    "void main() {\n"
    "    vec4 colKitten = texture(texKitten, Texcoord);\n"
    "    vec4 colPuppy = texture(texPuppy, Texcoord);\n"
    "    outColor = mix(colKitten, colPuppy, 0.5)\n"
    "        * vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n";

int main() {
    GLFWwindow * window = init();
    create_vao();
    create_vbo(sizeof(vertices), vertices);
    create_ebo(sizeof(elements), elements);
    GLuint vertex_shader = create_shader_or_exit(
        vertex_shader_src, GL_VERTEX_SHADER);
    GLuint frag_shader = create_shader_or_exit(
        frag_shader_src, GL_FRAGMENT_SHADER);
    GLuint program = create_program(vertex_shader, frag_shader);
    GLint pos_attr = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(
        pos_attr, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    GLint tex_attr = glGetAttribLocation(program, "texcoord");
    glEnableVertexAttribArray(tex_attr);
    glVertexAttribPointer(
        tex_attr, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
        reinterpret_cast<void *>(2 * sizeof(float)));
    GLint tex = create_texture_from_image("../sample.png", GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "texKitten"), 0);
    tex = create_texture_from_image("../sample2.png", GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(program, "texPuppy"), 1);
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        exit_on_gl_error();
    }
    glfwTerminate();
    return 0;
}
