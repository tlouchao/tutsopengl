cmake_minimum_required(VERSION 3.15...3.23)
project(tutsopengl LANGUAGES CXX)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/)
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)

add_executable(main main.cpp ${SOURCES})
target_link_libraries(main PRIVATE ${OPENGL_LIBRARY})
target_link_libraries(main PRIVATE glfw3dll)