#include "common.h"
#include "framebuffers_common.h"

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
    "    vec4 s1 = texture(\n"
    "        tex_frame_buffer, Texcoord - 1.0 / 300.0 - 1.0 / 200.0);\n"
    "    vec4 s2 = texture(\n"
    "        tex_frame_buffer, Texcoord + 1.0 / 300.0 - 1.0 / 200.0);\n"
    "    vec4 s3 = texture(\n"
    "        tex_frame_buffer, Texcoord - 1.0 / 300.0 + 1.0 / 200.0);\n"
    "    vec4 s4 = texture(\n"
    "        tex_frame_buffer, Texcoord + 1.0 / 300.0 + 1.0 / 200.0);\n"
    "    vec4 sx = 4.0 * ((s4 + s3) - (s2 + s1));\n"
    "    vec4 sy = 4.0 * ((s2 + s4) - (s1 + s3));\n"
    "    vec4 sobel = sqrt(sx * sx + sy * sy);\n"
    "    outColor = sobel;\n"
    "}\n";

int main() {
    return framebuffers_main(frag_shader_src_2d);
}
