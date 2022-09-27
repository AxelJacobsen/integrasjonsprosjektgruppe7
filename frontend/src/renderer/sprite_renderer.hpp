#pragma once
#include "ebo.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "../misc/structs.hpp"
#include "shader/shader.hpp"
#include "texture/texture.hpp"

class SpriteRenderer {
public:
	SpriteRenderer(glm::mat4 projection, int max_instances);

	void draw_sprite(glm::vec2 pos, float angle, glm::vec2 scale, Rect sprite_region);

	void draw_sprite(SpriteInfo sprite, glm::vec2 pos, float scale = 1.f, float rotation = 0.f) {
		draw_sprite(pos, rotation, glm::vec2{scale, scale}, Rect{sprite.offset, sprite.size});
	}

	void set_spritesheet(const Texture& spritesheet) {
		m_spritesheet = spritesheet;

		// We only need to bind once when we start to render using this spritesheet
		m_spritesheet.bind();
	}

	void render();

private:	
	struct SpriteVertex {
		glm::vec2 position{};
		glm::vec2 tex_coords{};
	};

	struct SpriteInstanceVertex {
		glm::vec2 grid_offset{};
		glm::vec2 grid_size{};
		glm::mat4 transformation{};
	};

	struct InstanceInfo {
		glm::vec2 position{ 0 };
		glm::vec2 scale{ 0 };
		float rotation{ 0 };
		glm::vec2 grid_offset{ 0 };
		glm::vec2 grid_size{ 0 };
	};

	InstanceInfo& next_instance() {
		VERIFY(m_instance_count < m_max_instance_count, "Instance limit reached!");
		return m_instances.at(m_instance_count++);
	}

	void update_transformations();

	static constexpr std::array<SpriteVertex, 4> static_vertices = {
		{
			//   Coord   |  TexCoord
			{{-0.5f, -0.5f}, {0.0f, 0.0f}}, // Lower left corner
			{{-0.5f,  0.5f}, {0.0f, 1.0f}}, // Upper left corner
			{{ 0.5f,  0.5f}, {1.0f, 1.0f}}, // Upper right corner
			{{ 0.5f, -0.5f}, {1.0f, 0.0f}}  // Lower right corner
		}
	};

	static constexpr GLuint static_indices[] = {
		0, 2, 1, // Upper triangle
		0, 3, 2 // Lower triangle
	};

private:
	Texture m_spritesheet{};
	Shader m_shader{};
	VAO m_vao{};
	VBO m_vertex_vbo{};
	VBO m_instance_vbo{};
	EBO m_ebo{};

	std::vector<InstanceInfo> m_instances{};
	std::vector<SpriteInstanceVertex> m_instance_data{};

	int m_max_instance_count{ 0 };
	int m_instance_count{0};
};
