cmake_minimum_required (VERSION 3.12)

project("mapproject")

# Add target binary
add_executable (${PROJECT_NAME} 
	"src/game/game.cpp"
	"src/main.cpp"
	"src/misc/stb.cpp"
	"src/misc/utils.hpp"
	"src/renderer/font_renderer.cpp"
	"src/renderer/sprite_renderer.cpp"
	"src/renderer/shader/shader.cpp"
	"src/renderer/texture/texture.cpp")

# Link third party libraries to target binary
target_link_libraries(${PROJECT_NAME} glm glfw glad OpenGL::GL freetype)

target_include_directories(${PROJECT_NAME} PRIVATE "third_party/" "third_party/freetype/include")

target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.hpp")

# Set C++ version to C++20
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_FORCE_SILENT_WARNINGS) # Warning from glm

# Enable warnings
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Add third party libraries
find_package(OpenGL REQUIRED)

add_subdirectory(third_party/glad)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glm)
add_subdirectory(third_party/freetype)
