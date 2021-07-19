#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "main.hpp"


class CShaderHandler {       
    public:             

    unsigned int compileShader(int type, const char* shaderSource)
    {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);
        // check for shader compile errors


        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return shader;
    }


    unsigned int makeShaderProgram(unsigned int shaders[1])
    {

        unsigned int shaderProgram = glCreateProgram();
        for (int i = 0; i < 2; i++)
        {
            glAttachShader(shaderProgram, shaders[i]);
        }
        
        glLinkProgram(shaderProgram);
    
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        for (int i = 0; i < 2; i++)
        {
            glDeleteShader(shaders[i]);
        }
    
        return shaderProgram;

    }

      
};