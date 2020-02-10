#version 450

/*layout (push_constant) uniform frag_ubo
{
	float fade_in;
} push_consts;*/

layout (set = 0, binding = 0) uniform sampler2D base_color_texture;
layout (set = 1, binding = 0) uniform frag_ubo
{
	float fade_in;
} frag_buffer;

layout (location = 0) in vec3 in_color;
layout (location = 1) in vec2 in_uv;

layout (location = 0) out vec4 out_color;

void main ()
{
	out_color = vec4 (in_color, 1) * texture (base_color_texture, in_uv) * frag_buffer.fade_in;
}