#include "common.h"

#include <cstdlib> // exit
#include <iostream> // cerr, endl

GLuint create_vao();
GLuint create_vbo(size_t size, float vertices[]);
GLuint create_shader(const GLchar * src, GLenum type);
void check_shader(GLuint id);
bool is_valid_shader(GLuint id);
GLuint create_program(GLuint vertex_shader, GLuint frag_shader);
void loop(GLFWwindow * window);

int main() {
    GLFWwindow * window = init();
    float vertices[] = {
         0.0f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,
    };
    create_vao();
    create_vbo(sizeof(vertices), vertices);
    const GLchar * vertex_shader_src =
        "#version 150 core\n"
        "\n"
        "in vec2 position;\n"
        "\n"
        "void main() {\n"
        "    gl_Position = vec4(position, 0.0, 1.0);\n"
        "}\n";
    GLuint vertex_shader = create_shader(vertex_shader_src, GL_VERTEX_SHADER);
    check_shader(vertex_shader);
    const GLchar * frag_shader_src =
        "#version 150 core\n"
        "\n"
        "out vec4 outColor;\n"
        "\n"
        "void main() {\n"
        "    outColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}\n";
    GLuint frag_shader = create_shader(frag_shader_src, GL_FRAGMENT_SHADER);
    check_shader(frag_shader);
    GLuint program = create_program(vertex_shader, frag_shader);
    loop(window);
    glfwTerminate();
    return 0;
}

GLuint create_vao() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

GLuint create_vbo(size_t size, float vertices[]) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return vbo;
}

GLuint create_shader(const GLchar * src, GLenum type) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    return id;
}

void check_shader(GLuint id) {
    if(!is_valid_shader(id)) {
        std::cerr
            << "glCompileShader(vertex_shader): failed, compile log follows"
            << std::endl;
        char buffer[512];
        glGetShaderInfoLog(id, 512, NULL, buffer);
        std::cerr << buffer;
        exit(1);
    }
}

bool is_valid_shader(GLuint id) {
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    return status == GL_TRUE;
}

GLuint create_program(GLuint vertex_shader, GLuint frag_shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, frag_shader);
    glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program);
    glUseProgram(program);
    GLint pos_attr = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    return program;
}

void loop(GLFWwindow * window) {
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        exit_on_gl_error();
    }
}
