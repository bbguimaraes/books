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
    "    outColor = texture(tex_frame_buffer, Texcoord);\n"
    "    float color = (outColor.x + outColor.y + outColor.z) / 3.0;\n"
    "    outColor = vec4(color, color, color, 1.0);\n"
    "}\n";

int main() {
    return framebuffers_main(frag_shader_src_2d);
}
