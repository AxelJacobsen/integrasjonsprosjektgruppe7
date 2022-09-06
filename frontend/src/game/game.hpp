#pragma once
#include <memory>
#include <array>

#include "../renderer/renderer.hpp"

class SpriteRenderer;

class Game {
public:
	void init(GLFWwindow* window);
	void update(float dt);
	void render();

private:
	enum SpriteID {
		PACMAN,
		GHOST
	};

	void draw_sprite(SpriteID sprite, glm::vec2 pos, float scale = 1.f) {
		m_sprite_renderer->draw_sprite(m_sprites.at(sprite), pos, scale);
	}

private:
	GLFWwindow* m_window{nullptr};
	std::unique_ptr<SpriteRenderer> m_sprite_renderer{nullptr};
	std::array<Sprite, 2> m_sprites{};

	float m_cursor_x{0}, m_cursor_y{0};
};
