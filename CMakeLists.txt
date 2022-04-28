cmake_minimum_required(VERSION 3.15...3.23)
project(tutsopengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# config openGL deps
find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_libraries(${CMAKE_SOURCE_DIR}/lib/libglfw3.a)
set(SOURCES ${CMAKE_SOURCE_DIR}/src/glad.c)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
target_link_libraries(OpenGL::GL)

# copy shader files to build directory
set(copy_shader_commands)
file(GLOB SHADERS ${CMAKE_SOURCE_DIR}/shaders/*.glsl)

foreach(shader ${SHADERS})
  list(APPEND copy_shader_commands
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${shader} ${CMAKE_BINARY_DIR}/shaders)
endforeach()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD 
    ${copy_shader_commands}
    COMMENT "Copying shaders to build directory"
)