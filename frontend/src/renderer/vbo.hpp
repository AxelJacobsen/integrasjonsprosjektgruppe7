#pragma once

class VBO {
public:
    VBO() = default;

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void initialize_and_bind() {
        glGenBuffers(1, &m_id);

        bind();
    }

    void remove() {
        glDeleteBuffers(1, &m_id);
    }

    template<typename T>
    void update(const T* buffer_data, const GLsizeiptr size, const GLintptr offset = 0) {
        // Transfer buffer data
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, buffer_data);
    }

    void allocate(const GLsizeiptr size) const {
        // Allocate buffer
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    }

private:
    GLuint m_id = 0;
};
