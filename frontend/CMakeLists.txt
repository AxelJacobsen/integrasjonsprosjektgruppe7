cmake_minimum_required (VERSION 3.12)

project("mapproject")

# Add target binary
add_executable (${PROJECT_NAME} "src/main.cpp" "src/renderer/renderer.cpp")

# Link third party libraries to target binary
target_link_libraries(${PROJECT_NAME} glm glfw glad OpenGL::GL)

# Set C++ version to C++20
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Add third party libraries
find_package(OpenGL REQUIRED)

add_subdirectory(third_party/glad)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glm)
