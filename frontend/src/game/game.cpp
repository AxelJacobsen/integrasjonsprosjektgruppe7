#include "game.hpp"

#include "../misc/settings.hpp"
#include "../renderer/texture/texture.hpp"
#include "entity/ecs.hpp"

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

	// Create ghost entities
	float size = 100.f;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			m_ghosts.push_back(create_ghost({ i * size + size / 2.f,j * size + size / 2.f }));
		}
	}

	// Create the pacman entity
	m_pacman = create_pacman();
}

void Game::update(float dt) {
	m_fps.update(dt);

	double x, y;
	glfwGetCursorPos(m_window, &x, &y);

	// Update pacman's position to the cursor position
	auto& pacman_position = m_registry.get_component<comp::Position>(m_pacman);
	pacman_position.pos = { x, y };

	auto rotation_view = m_registry.view<comp::Rotation, comp::Scale>();
	static float scale_timer = 0.f;
	scale_timer += dt;
	rotation_view.each([&](auto _, comp::Rotation& rotation, comp::Scale& scale) {
		rotation.angle += 180.f * dt;
		scale.scale = 100.f * std::sin(scale_timer);
	});
}

void Game::render() {
	auto view = m_registry.view<comp::Sprite, comp::Position>();
	view.each([this](auto entity, const comp::Sprite& sprite, const comp::Position& pos) {
		float rotation = 0.f;
		float scale = 100.f;
		if (m_registry.has_component<comp::Rotation>(entity)) {
			rotation = m_registry.get_component<comp::Rotation>(entity).angle;
		}
		if (m_registry.has_component<comp::Scale>(entity)) {
			scale = m_registry.get_component<comp::Scale>(entity).scale;
		}

		draw_sprite(sprite.info, pos.pos, scale, rotation);
	});

	m_sprite_renderer->render();
	m_font_renderer->draw_text(std::to_string(m_fps.fps()), {5,5}, 1.f);
}
