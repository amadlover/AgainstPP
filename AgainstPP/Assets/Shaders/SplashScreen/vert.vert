#version 450

layout (set = 0, binding = 0) uniform vert_ubo
{
	mat4 model_view_proj;
} mat_ubo;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_uv;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec2 out_uv;

void main ()
{
	gl_Position = mat_ubo.model_view_proj * vec4 (in_position, 1);

	out_color = vec4 (1, 1, 1, 1);
	out_uv = in_uv;
}