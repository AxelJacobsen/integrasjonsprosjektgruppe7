#include "sprite_renderer.hpp"

// TODO: attach sprites to entities, only update if marked as changed

SpriteRenderer::SpriteRenderer(glm::mat4 projection, int max_instances)
	// TODO: don't use relative paths like this
	: m_shader("../../../data/shaders/sprite.vert", "../../../data/shaders/sprite.frag"), m_max_instance_count(max_instances) {
	m_vao.initialize_and_bind();
	m_vertex_vbo.initialize_and_bind();
	m_instance_vbo.initialize_and_bind();
	m_ebo.initialize_and_bind();
	
	m_instance_data.resize(max_instances);
	m_instances.resize(max_instances);

	// Allocate VRAM for the data
	m_instance_vbo.allocate(static_cast<GLsizeiptr>(sizeof(SpriteInstanceVertex)) * max_instances);

	m_vertex_vbo.allocate(sizeof(SpriteVertex) * static_vertices.size());
	m_ebo.allocate(sizeof(static_indices));

	// Transfer the static vertex data
	m_vertex_vbo.update(static_vertices.data(), sizeof(static_vertices));
	// Transfer the index data
	m_ebo.update(static_indices, sizeof(static_indices));

	/*
	 * Tell OpenGL how to interpret the attributes
	*/
	m_vertex_vbo.bind();

	const auto vertex_stride = 2 * sizeof(glm::vec2);
	m_vao.link_attrib(0, 4, GL_FLOAT, GL_FALSE, vertex_stride, 0, false); // Vertex, NOT instanced


	m_instance_vbo.bind();

	const auto instance_stride = sizeof(SpriteInstanceVertex);
	m_vao.link_attrib(1, 4, GL_FLOAT, GL_FALSE, instance_stride, 0, true); // grid_offset, grid_size
	m_vao.link_attrib(2, 4, GL_FLOAT, GL_FALSE, instance_stride, 2 * sizeof(glm::vec2) + 0 * sizeof(glm::vec4), true); // transformation
	m_vao.link_attrib(3, 4, GL_FLOAT, GL_FALSE, instance_stride, 2 * sizeof(glm::vec2) + 1 * sizeof(glm::vec4), true); // ^
	m_vao.link_attrib(4, 4, GL_FLOAT, GL_FALSE, instance_stride, 2 * sizeof(glm::vec2) + 2 * sizeof(glm::vec4), true); // ^
	m_vao.link_attrib(5, 4, GL_FLOAT, GL_FALSE, instance_stride, 2 * sizeof(glm::vec2) + 3 * sizeof(glm::vec4), true); // ^

	m_shader.activate();
	m_shader.set_mat4("u_projection", projection);
}

void SpriteRenderer::draw_sprite(glm::vec2 pos, float angle, glm::vec2 scale,
                                 Rect sprite_region) {
	const auto grid_size = static_cast<glm::vec2>(m_spritesheet.size()) / sprite_region.size;
	const auto offset = sprite_region.pos / (static_cast<glm::vec2>(m_spritesheet.size()));

	auto& instance = next_instance();
	instance.position = pos;
	instance.rotation = angle;
	instance.scale = scale;
	instance.grid_offset = offset;
	instance.grid_size = grid_size;
}

void SpriteRenderer::update_transformations() {
	static constexpr glm::mat4 i_mat = glm::mat4(1);
	// TODO: only update when changed
	for (int i = 0; i < m_instance_count; i++) {
		const auto& instance = m_instances[i];

		// Calculate translation matrix
		const auto m_translation = glm::translate(i_mat, { instance.position.x, instance.position.y, 0.f });
		// Calculate the rotation matrix
		const auto m_rotation = glm::rotate(i_mat, glm::radians(instance.rotation), { 0.f, 0.f, 1.f });
		// Calculate the scale matrix
		const auto m_scale = glm::scale(i_mat, { instance.scale.x, instance.scale.y, 1.f });

		// Calculate the final model matrix
		const auto model = m_translation * m_rotation * m_scale;

		m_instance_data[i].grid_offset = instance.grid_offset;
		m_instance_data[i].grid_size = instance.grid_size;
		m_instance_data[i].transformation = model;
	}

	m_instance_vbo.bind();
	// Transfer buffer data to the GPU
	m_instance_vbo.update(m_instance_data.data(), static_cast<GLintptr>(m_instance_count * sizeof(SpriteInstanceVertex)));
}
void SpriteRenderer::render() {
	m_shader.activate();
	m_vao.bind();
	m_vertex_vbo.bind();
	m_instance_vbo.bind();
	m_ebo.bind();

	// Transfer buffer data to the GPU
	update_transformations();

	// Set the texture
	m_spritesheet.bind();
	m_spritesheet.activate();

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, m_instance_count);

	//m_vertex_buffer.clear();
	m_instance_count = 0;

	m_ebo.unbind();
	m_instance_vbo.unbind();
	m_vao.unbind();
}
