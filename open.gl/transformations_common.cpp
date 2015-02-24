#include "transformations_common.h"

#include "common.h"

const GLchar * FRAG_SHADER_SRC =
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

void setup_shader_attrs(GLuint program) {
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
}
