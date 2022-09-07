#include "font_renderer.hpp"

FontRenderer::FontRenderer(glm::mat4 projection) {
	m_vao.initialize_and_bind();
	m_vbo.initialize_and_bind();

	m_vbo.allocate(sizeof(float[6][4]));

	// Tell OpenGL how to interpret our vertex data
	m_vao.link_attrib(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	auto shader = Shader::create("../../../data/shaders/font.vert", "../../../data/shaders/font.frag");
	VERIFY(shader.has_value(), "Failed to load font shader!");

	m_shader = shader.value();

	// Set projection
	m_shader.set_mat4("projection", projection);

	// Enable blending 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FT_Library ft;
	VERIFY(!FT_Init_FreeType(&ft), "Failed to init FreeType Library!");

	FT_Face face;
	VERIFY(!FT_New_Face(ft, "../../../data/assets/fonts/arial.ttf", 0, &face), "Failed to load font!");

	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++) {

		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			logger::warning("Failed to load glyph: {}", c);
			continue;
		}

		// Generate the texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D,
		             0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
		             0, GL_RED,
		             GL_UNSIGNED_BYTE,
		             face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->ascender, face->descender,
			static_cast<unsigned>(face->glyph->advance.x)
		};

		m_characters.insert(std::make_pair(c, character));
	}
	// Free up the resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void FontRenderer::draw_text(std::string text, glm::vec2 pos, float scale, glm::vec3 color) {
	m_shader.activate();

	// Set color
	m_shader.set_3fv("text_color", color);
	glActiveTexture(GL_TEXTURE0);

	m_vao.bind();
	m_vbo.bind();

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		const auto& ch = m_characters.at(*c);

		const auto line_height = (static_cast<int>(ch.ascender - ch.descender) >> 6) * scale;

		float xpos = pos.x + ch.bearing.x * scale;
		float ypos = pos.y - ch.bearing.y * scale;
		ypos += line_height;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		// Update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f }, // Bottom left
            { xpos,     ypos,       0.0f, 0.0f }, // Top left
            { xpos + w, ypos,       1.0f, 0.0f }, // Top right

            { xpos,     ypos + h,   0.0f, 1.0f }, // Bottom left
            { xpos + w, ypos,       1.0f, 0.0f }, // Top right
            { xpos + w, ypos + h,   1.0f, 1.0f }  // Bottom right
        };

		// Bind the texture of the glyph
		glBindTexture(GL_TEXTURE_2D, ch.texture_id);

		m_vbo.update(vertices, sizeof(vertices));

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, 6);
		pos.x += (ch.advance >> 6) * scale;
	}

	m_vao.unbind();
	m_vbo.unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
}
