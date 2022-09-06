#pragma once

// NOTE: We only support a single texture slot atm
class Texture {
public:
	static std::optional<Texture> create(std::string_view texture_path);

	const glm::ivec2& size() const {
		return m_size;
	}

	void activate() const {
		// Set active texture slot (0)
		glActiveTexture(GL_TEXTURE0);
	}

	void bind() const {
		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
private:
	GLuint m_id{0};
	glm::ivec2 m_size{};
};