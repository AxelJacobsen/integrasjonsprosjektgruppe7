#pragma once
#include "../renderer/renderer.hpp"
#include "../renderer/font_renderer.hpp"

class FPS {
public:
	void update(float dt) {
		m_frames++;
		m_time += dt;
		if (m_time > 1.f) {
			m_fps = m_frames;
			m_frames = 0;
			m_time = 0.f;
		}
	}
	int fps() const {
		return m_fps;
	}
private:
	float m_time{ 0.f };
	int m_frames{ 0 };
	int m_fps{ 0 };
};

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
	std::unique_ptr<FontRenderer> m_font_renderer{nullptr};

	std::array<Sprite, 2> m_sprites{};

	float m_cursor_x{0}, m_cursor_y{0};
	FPS m_fps{};
};
