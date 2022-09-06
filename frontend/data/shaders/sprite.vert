#version 430 core

layout(location = 0) in vec4 vertex; // vec2 position and vec2 tex_coords

layout(location = 0) out vec2 o_texcoord;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform vec2 u_tex_grid_size;
uniform vec2 u_tex_offset;

void main() {
	o_texcoord = (vertex.zw / u_tex_grid_size) + u_tex_offset;

	gl_Position = u_projection * u_model * vec4(vertex.xy, 0.0, 1.0);
}
