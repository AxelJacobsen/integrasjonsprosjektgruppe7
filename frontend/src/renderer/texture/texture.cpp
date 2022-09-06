#include "texture.hpp"

#include <stb/stb_image.h>

std::optional<Texture> Texture::create(std::string_view texture_path) {
	int channels = 0, width, height;
	auto* pixels = stbi_load(texture_path.data(), &width, &height, &channels, 0);
	if (!pixels) {
		std::cerr << "Failed to load texture: " << texture_path << "\n";
		return {};
	}

	Texture texture;

	// Generate the texture
	glGenTextures(1, &texture.m_id);
	texture.m_size = glm::ivec2{ width, height };

	texture.activate();
	texture.bind();

	// Set up wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set up interpolation method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (channels < 3 || channels > 4) {
		std::cerr << "[TEXTURE] Failed to load " << texture_path << ", incorrect number of channels: " << channels << "\n";
		return {};
	}

	const auto format = channels == 4 ? GL_RGBA : GL_RGB;

	// Transfer the data to the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.m_size.x, texture.m_size.y, 0, format, GL_UNSIGNED_BYTE, pixels);

	// Generate multiple versions at different sizes (this might not be needed for this project)
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free the pixel data from RAM
	stbi_image_free(pixels);

	// Unbind the texture
	texture.unbind();

	return texture;
}
