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

# copy shader and texture files to build directory
set(dest_img ${CMAKE_BINARY_DIR}/img)
set(dest_shaders ${CMAKE_BINARY_DIR}/shaders)
file(GLOB src_shaders ${CMAKE_SOURCE_DIR}/shaders/*.glsl)

file(MAKE_DIRECTORY ${dest_img})
file(MAKE_DIRECTORY ${dest_shaders})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/img/bookshelf.png DESTINATION ${dest_img})

foreach(shader ${src_shaders})
  list(APPEND copy_shader_commands
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${shader} ${dest_shaders})
endforeach()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD 
    ${copy_shader_commands}
    DEPENDS ${src_shaders}
    COMMENT "Copying shaders to build directory"
)