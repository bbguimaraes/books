#include "geometry_common.h"
#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLchar * VERTEX_SHADER_SRC = GLSL(
    in vec3 pos;
    in vec3 color;
    out vec3 vColor;
    uniform mat4 proj;
    uniform mat4 view;
    void main() {
        gl_Position = proj * view * vec4(pos, 1.0);
        vColor = color;
    }
);

const GLchar * GEO_SHADER_SRC = GLSL(
    layout(points) in;
    layout(line_strip, max_vertices = 18) out;
    in vec3 vColor[];
    out vec3 fColor;
    uniform mat4 proj;
    uniform mat4 view;
    const float size = 0.1;
    void main() {
        fColor = vColor[0];
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size, -size,  size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4( size, -size,  size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4( size, -size, -size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size, -size, -size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size, -size,  size, 0.0);
        EmitVertex();
        EndPrimitive();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size,  size,  size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4( size,  size,  size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4( size,  size, -size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size,  size, -size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size,  size,  size, 0.0);
        EmitVertex();
        EndPrimitive();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size, -size, -size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size,  size, -size, 0.0);
        EmitVertex();
        EndPrimitive();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4( size, -size, -size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4( size,  size, -size, 0.0);
        EmitVertex();
        EndPrimitive();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4( size, -size,  size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4( size,  size,  size, 0.0);
        EmitVertex();
        EndPrimitive();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size, -size,  size, 0.0);
        EmitVertex();
        gl_Position =
            gl_in[0].gl_Position
                + proj * view * vec4(-size,  size,  size, 0.0);
        EmitVertex();
        EndPrimitive();
    }
);

const char * FRAG_SHADER_SRC = GLSL(
    in vec3 fColor;
    out vec4 outColor;
    void main() {
        outColor = vec4(fColor, 1.0);
    }
);

int main(int argc, char ** argv) {
    GLFWwindow * window = init();
    GLuint vertex_shader = create_shader(VERTEX_SHADER_SRC, GL_VERTEX_SHADER);
    GLuint frag_shader = create_shader(FRAG_SHADER_SRC, GL_FRAGMENT_SHADER);
    GLuint geo_shader = create_shader(GEO_SHADER_SRC, GL_GEOMETRY_SHADER);
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);
    glAttachShader(shader_program, geo_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    const float POINTS[] = {
        // pos              col
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(POINTS), POINTS, GL_STATIC_DRAW);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLint pos_attr = glGetAttribLocation(shader_program, "pos");
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(
        pos_attr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    GLint col_attr = glGetAttribLocation(shader_program, "color");
    glEnableVertexAttribArray(col_attr);
    glVertexAttribPointer(
        col_attr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        reinterpret_cast<void *>(3 * sizeof(float)));
    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);
    GLint proj_uni = glGetUniformLocation(shader_program, "proj");
    glUniformMatrix4fv(proj_uni, 1, GL_FALSE, glm::value_ptr(proj));
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 2.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    GLint view_uni = glGetUniformLocation(shader_program, "view");
    glUniformMatrix4fv(view_uni, 1, GL_FALSE, glm::value_ptr(view));
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)) {
        exit_on_gl_error();
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_POINTS, 0, 4);
    }
}
