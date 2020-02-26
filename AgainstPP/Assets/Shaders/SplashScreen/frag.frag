#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (set = 0, binding = 0) uniform frag_ubo
{
	float fade_in;
} frag_buffer;
layout (set = 0, binding = 1) uniform sampler2D base_color_texture;

layout (location = 0) in vec3 in_color;
layout (location = 1) in vec2 in_uv;

layout (location = 0) out vec4 out_color;

void main ()
{
	out_color = vec4 (in_color, 1) * texture (base_color_texture, in_uv) * frag_buffer.fade_in;
}