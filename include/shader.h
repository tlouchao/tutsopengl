#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {

public:

    Shader(const std::string& vname, const std::string& fname) {
        m_ID = linkShaderProgram(vname, fname);
    }
    ~Shader() noexcept {
        glDeleteProgram(m_ID);
    }
    void use() {
        glUseProgram(m_ID);
    }

private:

    std::string readShaderFile(const std::string& shaderFile) {
        std::string fdata;
        std::ifstream f;
        std::stringstream s;
        f.open(shaderFile);
        if (f.is_open()){
            s << f.rdbuf();
            f.close();
        } else {
            std::cerr << "Did not open file: " << shaderFile << '\n';
        }
        return s.str();
    }

    unsigned int compileShader(int success, char* infoLog, const int infoLen, const std::string& shaderFile, unsigned int shaderType){
        unsigned int shader = glCreateShader(shaderType);
        std::string shaderSrcTmp = readShaderFile(shaderFile);
        const char* shaderSrc = shaderSrcTmp.c_str();
        glShaderSource(shader, 1, &shaderSrc, NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success){
            std::cerr << "Source: \n\n" << shaderSrc << std::endl;
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            std::cerr << "Shader " + shaderFile + " compilation failed: " << infoLog << std::endl;
        }
        return shader;
    }

    unsigned int linkShaderProgram(const std::string& vname, const std::string& fname){

        int success;
        int infoLen = 1024;
        char infoLog[infoLen];

        unsigned int vertexShader = compileShader(success, infoLog, infoLen, vname, GL_VERTEX_SHADER);
        unsigned int fragmentShader = compileShader(success, infoLog, infoLen, fname, GL_FRAGMENT_SHADER);

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

    // Member variable
    unsigned int m_ID;

};