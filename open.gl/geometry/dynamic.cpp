#include "geometry_common.h"
#include "common.h"

const GLchar * VERTEX_SHADER_SRC = GLSL(
    in vec2 pos;
    in vec3 color;
    in float sides;
    out vec3 vColor;
    out float vSides;
    void main() {
        gl_Position = vec4(pos, 0.0, 1.0);
        vColor = color;
        vSides = sides;
    }
);

const GLchar * GEO_SHADER_SRC = GLSL(
    layout(points) in;
    layout(line_strip, max_vertices = 64) out;
    in vec3 vColor[];
    in float vSides[];
    out vec3 fColor;
    const float PI = 3.1415926;
    void main() {
        fColor = vColor[0];
        for(int i = 0; i <= vSides[0]; ++i) {
            float ang = PI * 2.0 / vSides[0] * i;
            vec4 offset = vec4(cos(ang) * 0.3, -sin(ang) * 0.4, 0.0, 0.0);
            gl_Position = gl_in[0].gl_Position + offset;
            EmitVertex();
        }
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
        // pos          col               sides
        -0.45f,  0.45f, 1.0f, 0.0f, 0.0f,  4.0f,
         0.45f,  0.45f, 0.0f, 1.0f, 0.0f,  8.0f,
         0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
        -0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(POINTS), POINTS, GL_STATIC_DRAW);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLint pos_attr = glGetAttribLocation(shader_program, "pos");
    glEnableVertexAttribArray(pos_attr);
    glVertexAttribPointer(
        pos_attr, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    GLint col_attr = glGetAttribLocation(shader_program, "color");
    glEnableVertexAttribArray(col_attr);
    glVertexAttribPointer(
        col_attr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        reinterpret_cast<void *>(2 * sizeof(float)));
    GLint sides_attr = glGetAttribLocation(shader_program, "sides");
    glEnableVertexAttribArray(sides_attr);
    glVertexAttribPointer(
        sides_attr, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        reinterpret_cast<void *>(5 * sizeof(float)));
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_POINTS, 0, 4);
    }
}
