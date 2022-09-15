#include "game.hpp"

#include "../misc/settings.hpp"
#include "../renderer/texture/texture.hpp"

void Game::init(GLFWwindow* window) {
	m_window = window;

	glm::mat4 projection = glm::ortho(0.f, settings::window_size.x,
	                                  settings::window_size.y, 0.f,
	                                  1.0f, -10.0f);

	m_sprite_renderer = std::make_unique<SpriteRenderer>(projection, settings::max_sprites);
	m_font_renderer = std::make_unique<FontRenderer>(projection);

	const auto spritesheet = Texture::create("../../../data/assets/pacman.png");
	VERIFY(spritesheet.has_value(), "Failed to load spritesheet");

	m_sprite_renderer->set_spritesheet(spritesheet.value());

	// NOTE: we can load this from a json file or something similar
	m_sprites[PACMAN] = Sprite{{10, 10 + (60 * 3) + (10 * 3)}, {60, 60}};
	m_sprites[GHOST] = Sprite{{300, 10 + (60 * 3) + (10 * 3)}, {60, 60}};
}

void Game::update(float dt) {
	double x, y;
	glfwGetCursorPos(m_window, &x, &y);

	m_cursor_x = static_cast<float>(x);
	m_cursor_y = static_cast<float>(y);

	m_fps.update(dt);
}

void Game::render() {
	m_font_renderer->draw_text(std::to_string(m_fps.fps()), {5,5}, 1.f);

	draw_sprite(GHOST, glm::vec2{100.f, 100.f}, 100.f);
	draw_sprite(PACMAN, glm::vec2{200.f, 100.f}, 100.f);
	draw_sprite(PACMAN, glm::vec2{m_cursor_x, m_cursor_y}, 100.f);

	m_sprite_renderer->render();
}
