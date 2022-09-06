#pragma once

class VAO {
public:
    VAO() = default;

    void initialize_and_bind() {
        glGenVertexArrays(1, &m_id);
        bind();
    }

    void link_attrib(GLuint layout, GLint num_components, GLenum type, GLboolean normalized, GLsizei stride, uintptr_t offset) {
        // Tell OpenGL how to interpret data at 'layout'
		glVertexAttribPointer(layout, num_components, type, normalized, stride,
			const_cast<const void*>(reinterpret_cast<void*>(offset)));

        // Enable the attribute
        glEnableVertexAttribArray(layout);
    }

    void bind() const {
		// Bind the vertex array
        glBindVertexArray(m_id);
    }

    void unbind() {
        // Set 0 as active buffer, unbinding the bound one
        glBindVertexArray(0);
    }

    void remove() const {
        glDeleteVertexArrays(1, &m_id);
    }

private:
    GLuint m_id = 0;
};
