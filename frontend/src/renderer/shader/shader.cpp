#include "shader.hpp"
#include "../../misc/utils.hpp"

GLuint create_shader(GLenum type, std::string_view source_path);


Shader::Shader(std::string_view vertex_src_path, std::string_view fragment_src_path) {
	const auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_src_path);
	const auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_src_path);

	// Create the shader program
	m_id = glCreateProgram();

	// Attach the shaders to the newly created shader program
	glAttachShader(m_id, vertex_shader);
	glAttachShader(m_id, fragment_shader);

	// Link the shader program
	glLinkProgram(m_id);

	// Clean up the shaders, we no longer need them after linking to the final shader program
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glUseProgram(m_id);
}

/*
 * Set of helper functions to transfer different data to the uniforms within the shader program
 */

void Shader::set_mat4(const std::string_view name, const glm::mat4& matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_1f(const std::string_view name, const float val) const {
	glUniform1f(glGetUniformLocation(m_id, name.data()), val);
}

void Shader::set_2fv(const std::string_view name, const glm::vec2& vec) const {
	glUniform2fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(vec));
}

void Shader::set_3fv(const std::string_view name, const glm::vec3& vec) const {
	glUniform3fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(vec));
}

void Shader::set_4fv(const std::string_view name, const glm::vec4& vec) const {
	glUniform4fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(vec));
}

void Shader::set_1i(const std::string_view name, const int val) const {
	glUniform1i(glGetUniformLocation(m_id, name.data()), val);
}

bool validate_compilation(GLuint shader_id) {
	GLint res;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res != GL_TRUE) {
		GLsizei log_len;
		GLchar log[1024];
		glGetShaderInfoLog(shader_id, sizeof(log), &log_len, log);

		logger::error("Shader info log: {}", log);
		return false;
	}
	return true;
}

GLuint create_shader(GLenum type, std::string_view source_path) {
	const auto shader = glCreateShader(type);
	const auto shader_source = utils::load_file(source_path);
	// TODO: Just give a warning?
	VERIFY(!shader_source.empty(), std::format("Failed to load shader source file {}", source_path));

	const auto* source = shader_source.data();
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	VERIFY(validate_compilation(shader));
	return shader;
}
