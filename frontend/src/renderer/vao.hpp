#pragma once

#ifndef glError
#define glError checkGlError(__FILE__,__LINE__)
#endif

inline void checkGlError(const char* file, int line) {
    GLenum err(glGetError());

    while (err != GL_NO_ERROR) {
        std::string error;
        switch (err) {
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            //case GL_INVALID_FRAMEBUFFER_OPERATION:
            //    error = "INVALID_FRAMEBUFFER_OPERATION";
            //    break;
            default:
                error = "Unknown error";
                break;
        }
        std::cout << file << " " << line << ": GL_" << error;
        err = glGetError();
    }
}
class VAO {
public:
    VAO() = default;

    void initialize_and_bind() {
        glGenVertexArrays(1, &m_id);
        bind();
    }

    void link_attrib(GLuint layout, GLint num_components, GLenum type, GLboolean normalized, GLsizei stride, uintptr_t offset, bool instanced = false) {
        // Tell OpenGL how to interpret data at 'layout'
		glVertexAttribPointer(layout, num_components, type, normalized, stride,
			const_cast<const void*>(reinterpret_cast<void*>(offset)));

        // Enable the attribute
        glEnableVertexAttribArray(layout);

		if (instanced) {
			glVertexAttribDivisor(layout, 1);
		}
		glError;
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
