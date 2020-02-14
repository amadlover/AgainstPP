#version 450

layout (set = 1, binding = 0) uniform sampler2D color_texture[4];
layout (push_constant) uniform image_index
{
    int index;
} constants;

layout (location = 0) in vec2 in_uv;

layout (location = 0) out vec4 out_color;

void main ()
{
    out_color = texture (color_texture[constants.index], in_uv);
}