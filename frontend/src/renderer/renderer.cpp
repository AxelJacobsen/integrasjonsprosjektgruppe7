#include "renderer.hpp"

SpriteRenderer::SpriteRenderer(glm::mat4 projection) {
	const GLfloat vertices[] = {
		//   Coord   |  TexCoord
		-0.5f, -0.5f, 0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 1.0f, 0.0f  // Lower right corner
	};

	const GLuint indices[] = {
		0, 2, 1, // Upper triangle
		0, 3, 2 // Lower triangle
	};
	m_vao.initialize_and_bind();
	m_vbo.initialize_and_bind();
	m_ebo.initialize_and_bind();

	// Allocate VRAM for the data
	m_vbo.allocate(sizeof(vertices));
	m_ebo.allocate(sizeof(indices));

	// Transfer the data to the GPU
	m_vbo.update(vertices, sizeof(vertices));
	m_ebo.update(indices, sizeof(indices));

	// Tell OpenGL how to interpret our vertex data
	m_vao.link_attrib(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	// TODO: don't use relative paths like this
	const auto shader = Shader::create("../../../data/shaders/sprite.vert", "../../../data/shaders/sprite.frag");
	VERIFY(shader.has_value(), "Failed to initialize the texture shader!");

	m_shader = shader.value();

	m_shader.activate();
	m_shader.set_mat4("u_projection", projection);
}

void SpriteRenderer::draw_sprite(glm::vec2 pos, float angle, glm::vec2 scale,
                                 Rect sprite_region) {
	m_shader.activate();

	const auto i_mat = glm::mat4(1.0f);

	// TODO: move these calculations to the GPU

	// Calculate translation matrix
	const auto m_translation = glm::translate(i_mat, {pos.x, pos.y, 0.f});

	// Calculate the rotation matrix
	const auto m_rotation = glm::rotate(i_mat, glm::radians(angle), {0.f, 0.f, 1.f});
	// Calculate the scale matrix
	const auto m_scale = glm::scale(i_mat, {scale.x, scale.y, 1.f});

	// Calculate the final model matrix
	const auto model = m_translation * m_rotation * m_scale;

	const auto grid_size = static_cast<glm::vec2>(m_spritesheet.size()) / sprite_region.size;
	const auto offset = sprite_region.pos / (static_cast<glm::vec2>(m_spritesheet.size()));

	m_spritesheet.activate();
	m_spritesheet.bind();
	// Transfer the uniforms to the GPU
	m_shader.set_mat4("u_model", model);
	m_shader.set_2fv("u_tex_grid_size", grid_size);
	m_shader.set_2fv("u_tex_offset", offset);

	m_vao.bind();
	m_vbo.bind();
	m_ebo.bind();

	glActiveTexture(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	m_vao.unbind();
	m_vbo.unbind();
	m_ebo.unbind();
}
