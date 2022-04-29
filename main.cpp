#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

GLFWwindow* glfwInitAndGetWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit())
    {
        std::cerr << "Failed to init\n" << std::endl;
    }

    GLFWwindow* window = glfwCreateWindow(900, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    return window;
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {

    std::cout << "Hello Universe!" << std::endl;
    GLFWwindow* window = glfwInitAndGetWindow();
    float vtx1[] = {
     -0.05f, -0.5f, 0.0f,   1.f, 0.f, 0.f,
     -0.05f, 0.5f, 0.0f,    0.f, 1.f, 0.f,
     -0.5f, 0.0f, 0.0f,     0.f, 0.f, 1.f
    };
    float vtx2[] = {
     0.05f, -0.5f, 0.0f,    1.f, 0.f, 1.f,
     0.05f, 0.5f, 0.0f,     1.f, 1.f, 0.f,
     0.5f, 0.0f, 0.0f,      0.f, 1.f, 1.f
    };
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // config vertex buffer object 1
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx1), vtx1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // config vertex buffer object 2
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx2), vtx2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /*
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    */

    Shader shaderProgram = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(.05f, .05f, .25f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        /*
        float timeValue = glfwGetTime();
        float uniValue = (std::cos(timeValue) + 1) / 2;
        int uniColorLocation = glGetUniformLocation(shaderProgram, "uniColor");
        glUniform4f(uniColorLocation, 0.0f, uniValue, uniValue, 1.0f);
        */
        shaderProgram.use();
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        shaderProgram.use();
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Exit" << std::endl;
    return 0;
}