#version 430

layout(location = 0) in vec2 o_texcoord;

layout(location = 0) out vec4 o_color;

layout(location = 1, binding = 0) uniform sampler2D u_texture;

void main() {
	vec4 color = vec4(1.f, 1.f, 1.f, 1.f);
	o_color = color * texture(u_texture, o_texcoord);

	// Discard if alpha below certain threshold
	if (o_color.a < 0.1)
		discard;
		// Use for debugging sprite
		//o_color = vec4(0.f, 1.f, 0.f, 1.f);
}
