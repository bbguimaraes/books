#include "framebuffers_common.h"
#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

const unsigned int vertices_count = 30;

const float vertices[] = {
    // pos            texcoords
    1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

const GLchar * framebuffers_vertex_shader_2d =
    "#version 150 core\n"
    "in vec3 position;\n"
    "in vec2 texcoord;\n"
    "out vec2 Texcoord;\n"
    "uniform mat4 proj;\n"
    "uniform mat4 view;\n"
    "void main() {\n"
    "    Texcoord = texcoord;\n"
    "    gl_Position = proj * view * vec4(position, 1.0);\n"
    "}\n";

const GLchar * frag_shader_src_2d =
    "#version 150 core\n"
    "\n"
    "in vec2 Texcoord;\n"
    "\n"
    "out vec4 outColor;\n"
    "\n"
    "uniform sampler2D tex_frame_buffer;\n"
    "\n"
    "void main() {\n"
    "    outColor = texture(tex_frame_buffer, Texcoord);\n"
    "}\n";

int main() {
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
    GLuint view_uni = glGetUniformLocation(program, "view");
    GLuint vao_2d = create_vao();
    GLuint vbo_2d = create_vbo(
        vertices_count * sizeof(float), vertices);
    GLuint vertex_shader_2d = create_shader_or_exit(
        framebuffers_vertex_shader_2d, GL_VERTEX_SHADER);
    GLuint frag_shader_2d = create_shader_or_exit(
        frag_shader_src_2d, GL_FRAGMENT_SHADER);
    GLuint program_2d = create_program(vertex_shader_2d, frag_shader_2d);
    GLint pos_attr_2d = glGetAttribLocation(program_2d, "position");
    glEnableVertexAttribArray(pos_attr_2d);
    glVertexAttribPointer(
        pos_attr_2d, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    GLint tex_attr_2d = glGetAttribLocation(program_2d, "texcoord");
    glEnableVertexAttribArray(tex_attr_2d);
    glVertexAttribPointer(
        tex_attr_2d, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
        reinterpret_cast<void *>(3 * sizeof(float)));
    glUniform1i(
        glGetUniformLocation(program, "tex_frame_buffer"),
        0);
    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);
    GLint proj_uni = glGetUniformLocation(program_2d, "proj");
    glUniformMatrix4fv(proj_uni, 1, GL_FALSE, glm::value_ptr(proj));
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    GLuint view_uni_2d = glGetUniformLocation(program_2d, "view");
    glUniformMatrix4fv(view_uni_2d, 1, GL_FALSE, glm::value_ptr(view));
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
        glm::mat4 view = glm::lookAt(
            glm::vec3(2.0f, 2.0f, -0.25f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(view_uni, 1, GL_FALSE, glm::value_ptr(view));
        draw_scene(angle, model_uni, ov_col_uni);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        view = glm::lookAt(
            glm::vec3(1.2f, 1.2f, 1.2f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(view_uni, 1, GL_FALSE, glm::value_ptr(view));
        draw_scene(angle, model_uni, ov_col_uni);
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
