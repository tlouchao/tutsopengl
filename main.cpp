#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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

std::string getShader(std::string filename) {
    std::ifstream f("./shaders/" + filename);
    std::stringstream s(std::ios_base::out);
    for (std::string line; std::getline(f, line); ) 
    {
        s << line << '\n';
    }
    return s.str();
}

unsigned int linkShaderProgram(std::string vname, std::string fname){

    int success;
    int infoLen = 1024;
    char infoLog[infoLen];

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSrc = getShader(vname + ".glsl").c_str();
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        std::cerr << "Vertex source: \n\n" << vertexSrc << std::endl;
        glGetShaderInfoLog(vertexShader, infoLen, NULL, infoLog);
        std::cerr << "Vertex shader " + vname + " compilation failed: " << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSrc = getShader(fname + ".glsl").c_str();
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        std::cerr << "Fragment source: \n\n" << fragmentSrc << std::endl;
        glGetShaderInfoLog(vertexShader, infoLen, NULL, infoLog);
        std::cerr << "Fragment shader " + fname + " compilation failed: " << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, infoLen, NULL, infoLog);
        std::cerr << "Shader program link failed: " << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

int main() {

    std::cout << "Hello Universe!" << std::endl;
    GLFWwindow* window = glfwInitAndGetWindow();

    float vtx1[] = {
     -0.1f, -0.5f, 0.0f,    1.f, 0.f, 0.f,
     -0.1f, 0.5f, 0.0f,     0.f, 1.f, 0.f,
     -0.5f, 0.0f, 0.0f,     0.f, 0.f, 1.f
    };
    float vtx2[] = {
     0.1f, -0.5f, 0.0f,     1.f, 1.f, 0.f,
     0.1f, 0.5f, 0.0f,      0.f, 1.f, 1.f,
     0.5f, 0.0f, 0.0f,      1.f, 0.f, 1.f
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

    unsigned int shaderProgram = linkShaderProgram("vertex", "fragment");

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(.05f, .05f, .25f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Exit" << std::endl;
    return 0;
}