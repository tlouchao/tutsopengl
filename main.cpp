#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <filesystem>
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

    std::cout << "Hello Universe!" << std::endl;
    GLFWwindow* window = glfwInitAndGetWindow();
    float vtx[] = {
     -0.5f, -0.5f, 0.0f,   1.f, 0.f, 0.f,   0.f, 0.f,
     -0.5f, 0.5f, 0.0f,    0.f, 1.f, 0.f,   0.f, 1.f,
     0.5f, 0.5f, 0.0f,     0.f, 0.f, 1.f,   1.f, 1.f,
     0.5f, -0.5f, 0.0f,    1.f, 1.f, 0.f,   1.f, 0.f
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx), vtx, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /*
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    */

    unsigned int textures[2];
    glGenTextures(2, textures);
    
    int width, height, nrChannels;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    std::string imgFileTmp = "img/bookshelf.png";
    const char* imgFile = imgFileTmp.c_str();
    stbi_set_flip_vertically_on_load(true);
    unsigned int ok = stbi_info(imgFile, &width, &height, &nrChannels);
    unsigned char* imgData = stbi_load(imgFile, &width, &height, &nrChannels, 0);

    if (imgData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << imgFile << " not loaded\n"; 
    }
    stbi_image_free(imgData);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    imgFileTmp = "img/blender_badge.png";
    imgFile = imgFileTmp.c_str();
    ok = stbi_info(imgFile, &width, &height, &nrChannels);
    imgData = stbi_load(imgFile, &width, &height, &nrChannels, 0);

    if (imgData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << imgFile << " not loaded\n"; 
    }
    stbi_image_free(imgData);

    Shader shaderProgram = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.5f));
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), 900.0f / 600.0f, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(.05f, .05f, .25f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        shaderProgram.setInt("uniTex1", 0);
        shaderProgram.setInt("uniTex2", 1);
        shaderProgram.setMat("model", model);
        shaderProgram.setMat("view", view);
        shaderProgram.setMat("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Exit" << std::endl;
    return 0;
}