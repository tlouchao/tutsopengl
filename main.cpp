#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <textures.h>
#include <VAOWrapper.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

GLFWwindow* glfwInitAndGetWindow(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit())
    {
        std::cerr << "Failed to init\n" << std::endl;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, false);
    gladLoadGL();
    return window;
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {

    const static int WIDTH = 900; const static int HEIGHT = 600;

    std::cout << "Hello Universe!" << std::endl;
    GLFWwindow* window = glfwInitAndGetWindow(WIDTH, HEIGHT);

    VAOWrapper VAOWrap = VAOWrapper();
    Shader shaderProgram = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Textures textures = Textures(std::vector<std::string>{"img/bookshelf.png", "img/blender_badge.png"}, 2);

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::rotate(model, glm::radians(45.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), 
                                  static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 
                                  0.1f, 
                                  100.0f);

    glEnable(GL_DEPTH_TEST); 
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(.05f, .05f, .25f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();
        shaderProgram.setInt("uniTex1", 0);
        shaderProgram.setInt("uniTex2", 1);
        shaderProgram.setMat("model", model);
        shaderProgram.setMat("view", view);
        shaderProgram.setMat("projection", projection);

        model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.75f, 0.66f, 0.5f));

        glBindVertexArray(VAOWrap.getVAO());
        glDrawArrays(GL_TRIANGLES, 0, VAOWrap.getIndices());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Exit" << std::endl;
    return 0;
}