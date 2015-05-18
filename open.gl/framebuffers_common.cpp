#include "framebuffers_common.h"
#include "common.h"

#include <ctime> // clock, CLOCKS_PER_SEC
#include <iostream> // cout, endl

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL/SOIL.h>

#include <GLFW/glfw3.h>

const GLchar * FRAMEBUFFERS_VERTEX_SHADER =
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

const GLchar * FRAMEBUFFERS_FRAG_SHADER =
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

const GLchar * FRAMEBUFFERS_VERTEX_SHADER_2D =
    "#version 150 core\n"
    "in vec2 position;\n"
    "in vec2 texcoord;\n"
    "out vec2 Texcoord;\n"
    "void main() {\n"
    "    Texcoord = texcoord;\n"
    "    gl_Position = vec4(position, 0.0, 1.0);\n"
    "}\n";

const unsigned int VERTICES_COUNT = 24;

const float VERTICES[] = {
    // pos        texcoords
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,

     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
};

void setup_shader_attrs(GLuint program) {
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
}

void create_frame_buffer_or_exit(
        unsigned int width, unsigned int height, GLuint * id, GLuint *tex) {
    create_frame_buffer(width, height, id, tex);
    if(check_frame_buffer(*id))
        return;
    std::cerr << "glCheckFramebufferStatus: not complete" << std::endl;
    std::exit(1);
}

void create_frame_buffer(
        unsigned int width, unsigned int height, GLuint * id, GLuint *tex) {
    glGenFramebuffers(1, id);
    glBindFramebuffer(GL_FRAMEBUFFER, *id);
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB,
        800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *tex, 0);
    GLuint rbo_depth_stencil;
    glGenRenderbuffers(1, &rbo_depth_stencil);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_stencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
        rbo_depth_stencil);
}

bool check_frame_buffer(GLuint frame_buffer) {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

int framebuffers_main(const GLchar * frag_shader_src_2d) {
    GLFWwindow * window = init();
    GLuint vao = create_vao();
    GLuint vbo = create_vbo(
        CUBE_VERTICES_COUNT * sizeof(float), CUBE_VERTICES);
    GLuint frame_buffer, tex_frame_buffer;
    create_frame_buffer_or_exit(800, 600, &frame_buffer, &tex_frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLuint vertex_shader = create_shader_or_exit(
        FRAMEBUFFERS_VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint frag_shader = create_shader_or_exit(
        FRAMEBUFFERS_FRAG_SHADER, GL_FRAGMENT_SHADER);
    GLuint program = create_program(vertex_shader, frag_shader);
    setup_shader_attrs(program);
    GLint tex_kitten = create_texture_from_image("../sample.png", GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "texKitten"), 0);
    GLint tex_puppy = create_texture_from_image("../sample2.png", GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(program, "texPuppy"), 1);
    GLint ov_col_uni = glGetUniformLocation(program, "overrideColor");
    setup_projection(program);
    GLint model_uni = glGetUniformLocation(program, "model");
    GLuint vao_2d = create_vao();
    GLuint vbo_2d = create_vbo(
        VERTICES_COUNT * sizeof(float), VERTICES);
    GLuint vertex_shader_2d = create_shader_or_exit(
        FRAMEBUFFERS_VERTEX_SHADER_2D, GL_VERTEX_SHADER);
    GLuint frag_shader_2d = create_shader_or_exit(
        frag_shader_src_2d, GL_FRAGMENT_SHADER);
    GLuint program_2d = create_program(vertex_shader_2d, frag_shader_2d);
    GLint pos_attr_2d = glGetAttribLocation(program_2d, "position");
    glEnableVertexAttribArray(pos_attr_2d);
    glVertexAttribPointer(
        pos_attr_2d, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    GLint tex_attr_2d = glGetAttribLocation(program_2d, "texcoord");
    glEnableVertexAttribArray(tex_attr_2d);
    glVertexAttribPointer(
        tex_attr_2d, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
        reinterpret_cast<void *>(2 * sizeof(float)));
    glUniform1i(
        glGetUniformLocation(program, "tex_frame_buffer"),
        0);
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
        glBindVertexArray(vao);
        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_kitten);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex_puppy);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float angle = generate_angle(
            static_cast<float>(std::clock()),
            static_cast<float>(CLOCKS_PER_SEC));
        draw_scene(angle, model_uni, ov_col_uni);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(vao_2d);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_frame_buffer);
        glUseProgram(program_2d);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_2d);
        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        exit_on_gl_error();
    }
    glfwTerminate();
    return 0;
}
