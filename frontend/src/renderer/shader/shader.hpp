#pragma once

class Shader {
public:
	static std::optional<Shader> create(std::string_view vertex_src_path, std::string_view fragment_src_path);

	void set_mat4(std::string_view name, const glm::mat4& matrix) const;
	void set_1f(std::string_view name, float val) const;
	void set_2fv(std::string_view name, const glm::vec2& vec) const;
	void set_3fv(std::string_view name, const glm::vec3& vec) const;
	void set_4fv(std::string_view name, const glm::vec4& vec) const;
	void set_1i(std::string_view name, int val) const;

	void activate() const { glUseProgram(m_id); }

private:
	GLuint m_id{0}; // Holds to shader program id
};
