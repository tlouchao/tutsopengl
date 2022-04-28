#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>

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

const char* getShader(std::string filename) {
    std::ifstream f;
    char* srcShader;
    int length;
    f.open("./shaders/" + filename);
    if (f.is_open()){
        f.seekg(0, std::ios::end);
        length = f.tellg();
        f.seekg(0, std::ios::beg);
        srcShader = new char[length];
        f.read(srcShader, length);
        f.close();
    } else {
        std::cerr << "Did not open file " + filename << std::endl;
    }
    return static_cast<const char*>(srcShader);
}

unsigned int linkShaderProgram(){

    int success;

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSrc = getShader("vertex.glsl");
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        std::cerr << "Vertex shader compilation failed" << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSrc = getShader("fragment.glsl");
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        std::cerr << "Fragment shader compilation failed" << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        std::cerr << "Shader program link failed" << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

int main() {

    std::cout << "Hello Universe!" << std::endl;
    GLFWwindow* window = glfwInitAndGetWindow();

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int shaderProgram = linkShaderProgram();

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //glClearColor(.2f, .2f, .5f, 1.f);
        //glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Exit" << std::endl;
    return 0;
}