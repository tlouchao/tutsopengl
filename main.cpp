#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <VAOWrapper.h>
#include <shader.h>
#include <textures.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

const static int WIDTH = 900; const static int HEIGHT = 600;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

bool firstMouse = true;
float yaw   = -90.0f;
float lastX =  static_cast<float>(WIDTH) / 2.0;

glm::vec3 cmPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cmFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cmUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float cmSpeed = 3.f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void processInput(GLFWwindow* window, float cmSpeed, glm::vec3& cmPos, glm::vec3& cmFront, glm::vec3& cmUp);

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
    gladLoadGL();
    return window;
}

int main() {

    std::cout << "Hello Universe!" << std::endl;

    GLFWwindow* window = glfwInitAndGetWindow(WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    VAOWrapper vWrapper = VAOWrapper();
    Shader shader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
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
    
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float cmSpeedAdjust = cmSpeed * deltaTime;
        processInput(window, cmSpeedAdjust, cmPos, cmFront, cmUp);

        glClearColor(.05f, .05f, .25f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(cmPos, cmPos + cmFront, cmUp);
        model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

        shader.use();
        shader.setInt("uniTex1", 0);
        shader.setInt("uniTex2", 1);
        shader.setMat("model", model);
        shader.setMat("view", view);
        shader.setMat("projection", projection);

        glBindVertexArray(vWrapper.getVAO());
        glDrawArrays(GL_TRIANGLES, 0, vWrapper.getIndices());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Exit" << std::endl;
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);

    if (firstMouse)
    {
        lastX = xpos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    lastX = xpos;

    float sensitivity = 0.05f; // change this value to your liking
    xoffset *= sensitivity;
    yaw += xoffset;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw));
    front.z = sin(glm::radians(yaw));
    cmFront = glm::normalize(front);
}

void processInput(GLFWwindow* window, float cmSpeed, glm::vec3& cmPos, glm::vec3& cmFront, glm::vec3& cmUp){

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cmPos += cmSpeed * cmFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cmPos -= cmSpeed * cmFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cmPos -= glm::normalize(glm::cross(cmFront, cmUp)) * cmSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cmPos += glm::normalize(glm::cross(cmFront, cmUp)) * cmSpeed;
}