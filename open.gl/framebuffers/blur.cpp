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
    "const float blur_size_h = 1.0 / 300.0;\n"
    "const float blur_size_v = 1.0 / 200.0;\n"
    "\n"
    "void main() {\n"
    "    vec4 sum = vec4(0.0);\n"
    "    for(int x = -4; x <= 4; x++)\n"
    "        for(int y = -4; y <= 4; y++)\n"
    "            sum += texture(\n"
    "                tex_frame_buffer,\n"
    "                vec2(\n"
    "                    Texcoord.x + x * blur_size_h,\n"
    "                    Texcoord.y + y * blur_size_v))\n"
    "                / 81.0;\n"
    "    outColor = sum;\n"
    "}\n";

int main() {
    return framebuffers_main(frag_shader_src_2d);
}
