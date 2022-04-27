#define GLFW_DLL
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    std::cout << "Hello Universe!" << std::endl;
    std::cout << glfwInit() << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit())
    {
        std::cerr << "No Init\n" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(400, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    glfwTerminate();
    std::cout << "Exit" << std::endl;
    return 0;
}