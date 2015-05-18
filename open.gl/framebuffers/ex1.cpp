#include "framebuffers_common.h"
#include "common.h"

#include <GLFW/glfw3.h>

const GLchar * frag_shader_src_2d_0 =
    "#version 150 core\n"
    "\n"
    "in vec2 Texcoord;\n"
    "\n"
    "out vec4 outColor;\n"
    "\n"
    "uniform sampler2D tex_frame_buffer_0;\n"
    "\n"
    "const float s = 1.0 / 300.0;\n"
    "\n"
    "void main() {\n"
    "    vec4 sum = vec4(0.0);\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x - 3 * s, Texcoord.y)) * 0.0009258546453264528;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x - 2 * s, Texcoord.y)) * 0.029464011967309193;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x - 1 * s, Texcoord.y)) * 0.23493154005411251;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x, Texcoord.y)) * 0.4693438592958032;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x + 1 * s, Texcoord.y)) * 0.23493154005411251;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x + 2 * s, Texcoord.y)) * 0.029464011967309193;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x + 3 * s, Texcoord.y)) * 0.0009258546453264528;\n"
    "    outColor = sum;\n"
    "}\n";

const GLchar * frag_shader_src_2d_1 =
    "#version 150 core\n"
    "\n"
    "in vec2 Texcoord;\n"
    "\n"
    "out vec4 outColor;\n"
    "\n"
    "uniform sampler2D tex_frame_buffer_0;\n"
    "\n"
    "const float s = 1.0 / 300.0;\n"
    "\n"
    "void main() {\n"
    "    vec4 sum = vec4(0.0);\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x, Texcoord.y - 3 * s)) * 0.0009258546453264528;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x, Texcoord.y - 2 * s)) * 0.029464011967309193;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x, Texcoord.y - 1 * s)) * 0.23493154005411251;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x, Texcoord.y)) * 0.4693438592958032;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x, Texcoord.y + 1 * s)) * 0.23493154005411251;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x, Texcoord.y + 2 * s)) * 0.029464011967309193;\n"
    "    sum += texture(\n"
    "        tex_frame_buffer_0,\n"
    "        vec2(Texcoord.x, Texcoord.y + 3 * s)) * 0.0009258546453264528;\n"
    "    outColor = sum;\n"
    "}\n";

int main() {
    GLFWwindow * window = init();
    GLuint vao = create_vao();
    GLuint vbo = create_vbo(
        CUBE_VERTICES_COUNT * sizeof(float), CUBE_VERTICES);
    GLuint frame_buffer_0, frame_buffer_1;
    GLuint tex_frame_buffer_0, tex_frame_buffer_1;
    create_frame_buffer_or_exit(
        800, 600, &frame_buffer_0, &tex_frame_buffer_0);
    create_frame_buffer_or_exit(
        800, 600, &frame_buffer_1, &tex_frame_buffer_1);
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
    GLuint vao_2d_0 = create_vao();
    GLuint vbo_2d_0 = create_vbo(
        VERTICES_COUNT * sizeof(float), VERTICES);
    GLuint vertex_shader_2d_0 = create_shader_or_exit(
        FRAMEBUFFERS_VERTEX_SHADER_2D, GL_VERTEX_SHADER);
    GLuint frag_shader_2d_0 = create_shader_or_exit(
        frag_shader_src_2d_0, GL_FRAGMENT_SHADER);
    GLuint program_2d_0 = create_program(vertex_shader_2d_0, frag_shader_2d_0);
    GLint pos_attr_2d_0 = glGetAttribLocation(program_2d_0, "position");
    glEnableVertexAttribArray(pos_attr_2d_0);
    glVertexAttribPointer(
        pos_attr_2d_0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    GLint tex_attr_2d_0 = glGetAttribLocation(program_2d_0, "texcoord");
    glEnableVertexAttribArray(tex_attr_2d_0);
    glVertexAttribPointer(
        tex_attr_2d_0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
        reinterpret_cast<void *>(2 * sizeof(float)));
    glUniform1i(
        glGetUniformLocation(program, "tex_frame_buffer_0"),
        0);
    GLuint vao_2d_1 = create_vao();
    GLuint vbo_2d_1 = create_vbo(
        VERTICES_COUNT * sizeof(float), VERTICES);
    GLuint vertex_shader_2d_1 = create_shader_or_exit(
        FRAMEBUFFERS_VERTEX_SHADER_2D, GL_VERTEX_SHADER);
    GLuint frag_shader_2d_1 = create_shader_or_exit(
        frag_shader_src_2d_1, GL_FRAGMENT_SHADER);
    GLuint program_2d_1 = create_program(vertex_shader_2d_1, frag_shader_2d_1);
    GLint pos_attr_2d_1 = glGetAttribLocation(program_2d_1, "position");
    glEnableVertexAttribArray(pos_attr_2d_1);
    glVertexAttribPointer(
        pos_attr_2d_1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    GLint tex_attr_2d_1 = glGetAttribLocation(program_2d_1, "texcoord");
    glEnableVertexAttribArray(tex_attr_2d_1);
    glVertexAttribPointer(
        tex_attr_2d_1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
        reinterpret_cast<void *>(2 * sizeof(float)));
    glUniform1i(
        glGetUniformLocation(program, "tex_frame_buffer_1"),
        0);
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_0);
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

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_1);
        glBindVertexArray(vao_2d_0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_frame_buffer_0);
        glUseProgram(program_2d_0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_2d_0);
        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(vao_2d_1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_frame_buffer_1);
        glUseProgram(program_2d_1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_2d_1);
        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        exit_on_gl_error();
    }
    glfwTerminate();
    return 0;
}
