#pragma once
#include "../renderer/sprite_renderer.hpp"
#include "../renderer/font_renderer.hpp"
#include "entity/ecs.hpp"
#include "entity/components.hpp"

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
	void draw_sprite(SpriteInfo sprite, glm::vec2 pos, float scale = 1.f, float rotation = 0.f) {
		m_sprite_renderer->draw_sprite(sprite, pos, scale, rotation);
	}

	ecs::Entity create_ghost(glm::vec2 pos) {
		// Create the entity
		const auto ghost = m_registry.create_entity();
		// Create the sprite (with the offset within the spritesheet and the size)
		const auto sprite = SpriteInfo{{300, 10 + (60 * 3) + (10 * 3)}, {60, 60}};
		// Add the sprite component
		m_registry.add_component(ghost, comp::Sprite{ sprite });
		// Add the position component
		m_registry.add_component(ghost, comp::Position{ pos });
		m_registry.add_component(ghost, comp::Rotation{ 0.f });
		m_registry.add_component(ghost, comp::Scale{ });
		return ghost;
	}

	ecs::Entity create_pacman() {
		// See create_ghost for documentation ^
		const auto pacman = m_registry.create_entity();
		const auto sprite = SpriteInfo{{10, 10 + (60 * 3) + (10 * 3)}, {60, 60}};
		m_registry.add_component(pacman, comp::Sprite{ sprite});
		m_registry.add_component(pacman, comp::Position{ {0,0 } });
		return pacman;
	}
private:
	GLFWwindow* m_window{nullptr};
	FPS m_fps{};

	std::unique_ptr<SpriteRenderer> m_sprite_renderer{nullptr};
	std::unique_ptr<FontRenderer> m_font_renderer{nullptr};
	ecs::Registry m_registry{};

	ecs::Entity m_pacman{};
	std::vector<ecs::Entity> m_ghosts{};
};
