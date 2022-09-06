#include "shader.hpp"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "../../misc/utils.hpp"


bool validate_compilation(GLuint);

std::optional<Shader> Shader::create(std::string_view vertex_src_path, std::string_view fragment_src_path) {
	const auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const auto vertex_source = utils::load_file(vertex_src_path);
	if (vertex_source.empty()) {
		std::cerr << "Failed to load vertex source file " << vertex_src_path << "\n";
		return {};
	}
	const auto* vss = vertex_source.data();
	glShaderSource(vertex_shader, 1, &vss, nullptr);
	glCompileShader(vertex_shader);
	if (!validate_compilation(vertex_shader)) {
		std::cerr << "Failed to compile the vertex shader: " << vertex_src_path << "\n";
		return {};
	}

	const auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const auto fragment_source = utils::load_file(fragment_src_path);
	if (fragment_source.empty()) {
		std::cerr << "Failed to load fragment source file " << fragment_src_path << "\n";
		return {};
	}

	const auto* fss = fragment_source.data();
	glShaderSource(fragment_shader, 1, &fss, nullptr);
	glCompileShader(fragment_shader);
	if (!validate_compilation(fragment_shader)) {
		std::cerr << "Failed to compile the fragment shader: " << fragment_src_path << "\n";
		return {};
	}
	Shader shader;
	// Create the shader program
	shader.m_id = glCreateProgram();

	// Attach the shaders to the newly created shader program
	glAttachShader(shader.m_id, vertex_shader);
	glAttachShader(shader.m_id, fragment_shader);

	// Link the shader program
	glLinkProgram(shader.m_id);

	// Clean up the shaders, we no longer need them after linking to the final shader program
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glUseProgram(shader.m_id);

	return shader;
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

		std::cerr << "[SHADER] Shader info log: " << log << "\n";
		return false;
	}
	return true;
}
