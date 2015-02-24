#include "common.h"

float vertices[] = {
    // pos        color             texcoords
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
};

GLuint elements[] = {
    0, 1, 2,
    2, 3, 0,
};

const GLchar * vertex_shader_src =
    "#version 150 core\n"
    "\n"
    "in vec2 position;\n"
    "in vec3 color;\n"
    "in vec2 texcoord;\n"
    "\n"
    "out vec3 Color;\n"
    "out vec2 Texcoord;\n"
    "\n"
    "void main() {\n"
    "    Color = color;\n"
    "    Texcoord = texcoord;\n"
    "    gl_Position = vec4(position, 0.0, 1.0);\n"
    "}\n";

const GLchar * frag_shader_src =
    "#version 150 core\n"
    "\n"
    "in vec3 Color;\n"
    "in vec2 Texcoord;\n"
    "\n"
    "out vec4 outColor;\n"
    "\n"
    "uniform sampler2D tex;\n"
    "\n"
    "void main() {\n"
    "    outColor = texture(tex, Texcoord) * vec4(Color, 1.0);\n"
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
        pos_attr, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
    GLint col_attr = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(col_attr);
    glVertexAttribPointer(
        col_attr, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
        reinterpret_cast<void *>(2 * sizeof(float)));
    GLint tex_attr = glGetAttribLocation(program, "texcoord");
    glEnableVertexAttribArray(tex_attr);
    glVertexAttribPointer(
        tex_attr, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
        reinterpret_cast<void *>(5 * sizeof(float)));
    GLint tex = create_texture_from_image("../sample.png", GL_TEXTURE0);
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
