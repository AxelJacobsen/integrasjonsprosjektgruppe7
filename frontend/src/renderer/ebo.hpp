#pragma once

class EBO {
public:
    EBO() = default;

    void bind() const {
        // Set active buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void unbind() {
        // Set 0 as active buffer, unbinding the bound one
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void initialize_and_bind() {
        // Generate the buffer
        glGenBuffers(1, &m_id);

        bind();
    }

    void allocate(const GLsizeiptr size) const {
        // Bind/set as active EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);

        // Transfer the data into the buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    }

    void update(const GLuint* indices, const GLsizeiptr size, const GLintptr offset = 0) const {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, indices);
    }

    void remove() const {
        glDeleteBuffers(1, &m_id);
    }

private:
    GLuint m_id{0};
};
