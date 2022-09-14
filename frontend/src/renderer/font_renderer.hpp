#pragma once
#include "../renderer/shader/shader.hpp"
#include "../renderer/vao.hpp"
#include "../renderer/vbo.hpp"
#include "../renderer/ebo.hpp"

class FontRenderer {
public:
	FontRenderer(glm::mat4 projection);
	void draw_text(std::string text, glm::vec2 pos, float scale = 1.f, glm::vec3 color = {1,1,1});
private:
	struct Character {
		unsigned int texture_id; // ID handle of the glyph texture
		glm::ivec2 size;         // Size of glyph
		glm::ivec2 bearing;      // Offset from baseline to left/top of glyph
		short ascender;
		short descender;
		unsigned int advance;    // Offset to advance to next glyph
	};
	std::map<char, Character> m_characters{};
	Shader m_shader{};
	VAO m_vao{};
	VBO m_vbo{};
};
