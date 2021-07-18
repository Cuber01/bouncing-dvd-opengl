#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstring>

#include "shader.cpp"
#include "glfw.cpp"
#include "utils.cpp"


// set up vertex data (and buffer(s)) and configure vertex attributes
float vertices[] = {

    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

};



int success;
char infoLog[512];

const char* fragmentShaderPath = "./res/fragment_shader.fs";
const char* vertexShaderPath = "./res/vertex_shader.vs";

#define BUFFER_SIZE 2000

char sbuffer[BUFFER_SIZE];




unsigned int processVerticesData()
{
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

void Render()
{
    
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

   // std::cout << std::endl << "--- Compile: ----------------------------------------------" << std::endl;  

    unsigned int fragmentShader = ShaderHandler.compileShader(GL_FRAGMENT_SHADER , sbuffer );

    memset(sbuffer, 0, sizeof(sbuffer));    
    n = Utils.readFile(vertexShaderPath,sbuffer, BUFFER_SIZE );
    

    std::cout << std::endl << "--- Compile: ----------------------------------------------" << std::endl;  

    unsigned int vertexShader  = ShaderHandler.compileShader(GL_VERTEX_SHADER , sbuffer );

    unsigned int shaders[] = {fragmentShader, vertexShader};
    unsigned int shaderProgram = ShaderHandler.makeShaderProgram(shaders);

    unsigned int VAO = processVerticesData();


    glUseProgram(shaderProgram);

    // render loop
    while (!glfwWindowShouldClose(GlfwHandler.window))
    {
        // input
        GlfwHandler.processInput(GlfwHandler.window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(GlfwHandler.window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
