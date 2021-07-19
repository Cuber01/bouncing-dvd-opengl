#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstring>

#include "shader.cpp"
#include "glfw.cpp"
#include "utils.cpp"

#include "stb_image.h"


int success;
char infoLog[512];

const char* fragmentShaderPath = "./res/fragment_shader.fs";
const char* vertexShaderPath = "./res/vertex_shader.vs";

#define BUFFER_SIZE 2000

char sbuffer[BUFFER_SIZE];

struct rvStruct {
    unsigned int data1;
    unsigned int data2;
};

// set up vertex data (and buffer(s)) and configure vertex attributes
float vertices[] = {

     // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 

};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

rvStruct processData()
{
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 



    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    rvStruct rvStruct {.data1 = VAO, .data2 = EBO};

    return rvStruct;
}

void Render(CGlfwHandler GlfwHandler, unsigned int VAO)
{
    // input
    GlfwHandler.processInput(GlfwHandler.window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // render the triangle
    glBindVertexArray(VAO);
    
    // glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glfwSwapBuffers(GlfwHandler.window);
    glfwPollEvents();
   
}

int main()
{
    int n;

    CShaderHandler ShaderHandler;
    CGlfwHandler GlfwHandler;
    CUtils Utils;

    GlfwHandler._glfwInit();

   
    
    memset(sbuffer, 0, sizeof(sbuffer));
    n = Utils.readFile(fragmentShaderPath,sbuffer, BUFFER_SIZE );

    unsigned int fragmentShader = ShaderHandler.compileShader(GL_FRAGMENT_SHADER , sbuffer );

    memset(sbuffer, 0, sizeof(sbuffer));    
    n = Utils.readFile(vertexShaderPath,sbuffer, BUFFER_SIZE );
    

    unsigned int vertexShader  = ShaderHandler.compileShader(GL_VERTEX_SHADER , sbuffer );

    unsigned int shaders[] = {fragmentShader, vertexShader};
    unsigned int shaderProgram = ShaderHandler.makeShaderProgram(shaders);

    rvStruct _data = processData();  
    unsigned int VAO = _data.data1;
    unsigned int EBO = _data.data2;


    glUseProgram(shaderProgram);




    // TODO here
    // load texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("./res/container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    //    load texture end

    // TODO this causes segfault
    // stbi_image_free(data);


    while (!glfwWindowShouldClose(GlfwHandler.window))
    {
        Render(GlfwHandler, VAO);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    

    glfwTerminate();
    return 0;
}
