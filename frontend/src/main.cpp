#include "misc/settings.hpp"
#include "game/game.hpp"


void error_callback(int, const char*);
void GLAPIENTRY message_callback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*);

int main() {
#ifdef _DEBUG
	// Set error callback
	glfwSetErrorCallback(&error_callback);
#endif

	// Initialize GLFW
	VERIFY(glfwInit());

	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Create the window
	auto* window = glfwCreateWindow(static_cast<int>(settings::window_size.x),
	                                static_cast<int>(settings::window_size.y), "Map game", nullptr, nullptr);
	VERIFY(window);

	// Set the window as the current context
	glfwMakeContextCurrent(window);

	// Disable vsync
	glfwSwapInterval(0);

	// Initialize GLAD
	VERIFY(gladLoadGL() == GL_TRUE);

#ifdef _DEBUG
	// Enable debug output and set the message callback handler
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(message_callback, nullptr);
#endif


	// Initialize the game
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
	logger::error("[GLFW] {} : {}", code, description);
}

void GLAPIENTRY message_callback(GLenum source,
                                 GLenum type,
                                 GLuint id,
                                 GLenum severity,
                                 GLsizei length,
                                 const GLchar* message,
                                 const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR) {
		logger::error("[GL] type {}, {}", type, message);
	} else {
		logger::warning("[GL] type {}, {}", type, message);
	}
}
