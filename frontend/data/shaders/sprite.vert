#version 430 core

layout(location = 0) in vec4 vertex; // vec2 position and vec2 tex_coords

/*
 * Instanced
*/
layout(location = 1) in vec4 tex_info; // vec2 texture_offset and vec2 grid_size
layout(location = 2) in mat4 transformation_matrix;

layout(location = 0) out vec2 o_texcoord; // output texture coord to fragment shader

uniform mat4 u_projection;

void main() {
	o_texcoord = (vertex.zw / tex_info.zw) + tex_info.xy;
	gl_Position = u_projection * transformation_matrix * vec4(vertex.xy, 0.0, 1.0);
}
