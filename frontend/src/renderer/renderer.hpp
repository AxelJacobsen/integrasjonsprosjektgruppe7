#pragma once
#include "ebo.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "../misc/structs.hpp"
#include "shader/shader.hpp"
#include "texture/texture.hpp"

struct GLFWwindow;

struct Sprite {
	glm::vec2 offset{};
	glm::vec2 size{};
};

class SpriteRenderer {
public:
	SpriteRenderer(glm::mat4 projection);

	void draw_sprite(glm::vec2 pos, float angle, glm::vec2 scale, Rect sprite_region);

	void draw_sprite(Sprite sprite, glm::vec2 pos, float scale = 1.f) {
		draw_sprite(pos, 0.f, glm::vec2{scale, scale}, Rect{sprite.offset, sprite.size});
	}

	void set_spritesheet(const Texture& spritesheet) {
		m_spritesheet = spritesheet;

		// We only need to bind once when we start to render using this spritesheet
		m_spritesheet.bind();
	}

private:
	Texture m_spritesheet{};
	Shader m_shader{};
	VAO m_vao{};
	VBO m_vbo{};
	EBO m_ebo{};
};
