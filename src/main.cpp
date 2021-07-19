#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <iostream>
#include <cstring>

#include "shader.cpp"
#include "glfw.cpp"
#include "utils.cpp"
#include "texture.cpp"

int success;
char infoLog[512];

float dvd_x = 0.5f;
float dvd_y = -0.5f;


const char* fragmentShaderPath = "./res/shaders/fragment_shader.fs";
const char* vertexShaderPath = "./res/shaders/vertex_shader.vs";
    
const char* containerTexturePath = "./res/images/container.jpg";
const char* smileyTexturePath = "./res/images/smiley.png";

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

unsigned int indices[] = { 
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
void Render(CGlfwHandler GlfwHandler, unsigned int VAO, unsigned int shaderProgram, glm::mat4 trans)
{
    // input
    GlfwHandler.processInput(GlfwHandler.window);

    dvd_x += 0.01;
    dvd_y += 0.01;

    // clear
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //trans
    trans = glm::translate(trans, glm::vec3(dvd_x, dvd_y, 0.0f));
   // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans)); 
    
    // render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // swap buffers, process events
    glfwSwapBuffers(GlfwHandler.window);
    glfwPollEvents();
   
}

int main()
{
    int n;

    CShaderHandler ShaderHandler;
    CGlfwHandler GlfwHandler;
    CUtils Utils;
    CTextureHandler TextureHandler;
    

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

    TextureHandler.textureLoad(smileyTexturePath, GL_RGBA, false); 

    glm::mat4 trans = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(GlfwHandler.window))
    {
        Render(GlfwHandler, VAO, shaderProgram, trans);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    

    glfwTerminate();
    return 0;
}
