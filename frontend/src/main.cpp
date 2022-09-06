#include <iostream>
#include "misc/settings.hpp"
#include "renderer/renderer.hpp"
#include "game/game.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

void error_callback(int, const char*);
void GLAPIENTRY message_callback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*);

int main() {
#ifdef _DEBUG
	// Set error callback
	glfwSetErrorCallback(&error_callback);
#endif

	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW!\n";
		return EXIT_FAILURE;
	}
	
	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Create the window
	auto* window = glfwCreateWindow(static_cast<int>(settings::window_size.x), static_cast<int>(settings::window_size.y), "Map game", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create the window!\n";
		return EXIT_FAILURE;
	}

	// Set the window as the current context
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (gladLoadGL() != GL_TRUE) {
		std::cerr << "Failed to load GLAD!\n";
		return EXIT_FAILURE;
	}

#ifdef _DEBUG
	// Enable debug output and set the message callback handler
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(message_callback, nullptr);
#endif


	Game game;
	game.init(window);


	glViewport(0, 0, static_cast<int>(settings::window_size.x), static_cast<int>(settings::window_size.y));
	glClearColor(0.5f, 0.1f, 0.1f, 1.0f);

	glfwSetTime(0.0);
	auto last_time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		// Poll events such as input
		glfwPollEvents();
		// Clear buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Calculate time delta from the last frame to now
		const auto current_time = glfwGetTime();
		const auto dt = current_time - last_time;
		last_time = current_time;

		// Update game
		game.update(static_cast<float>(dt));

		// Render game
		game.render();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

/// Handle error events
void error_callback(int code, const char* description) {
	std::cerr << "[GLFW] " << code << ": " << description;
}

void GLAPIENTRY message_callback(GLenum source,
                                 GLenum type,
                                 GLuint id,
                                 GLenum severity,
                                 GLsizei length,
                                 const GLchar* message,
                                 const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR) {
		std::cerr << "[GL CALLBACK] type " << type << ", " << "severity = " << severity << ", " << "message = " << message << "\n";
	} else {
		std::cout << "[GL CALLBACK][WARN] type " << type << ", " << "severity = " << severity << ", " << "message = " << message << "\n";
	}
}
