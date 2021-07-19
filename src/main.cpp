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

// debug
int success;
char infoLog[512];

// dvd

void dvd_update();

float dvd_x = 0.0;
float dvd_y = 0.0;

int dvd_vel_x = 1;
int dvd_vel_y = 1;

float dvd_speed = 0.007;

float dvd_width = 0.7; // hard coded, change that
float dvd_height = 0.46;

// paths
const char* fragmentShaderPath = "./res/shaders/fragment_shader.fs";
const char* vertexShaderPath = "./res/shaders/vertex_shader.vs";
    
const char* dvdTexturePath = "./res/images/dvd.png";

// other
#define BUFFER_SIZE 2000

char sbuffer[BUFFER_SIZE];

struct rvStruct {
    unsigned int data1;
    unsigned int data2;
};


// my aspect ratio: 2.13:1
float vertices[] = {

     // positions             // colors           // texture coords
     0.7f,   0.0f,  0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.7f,  -0.46f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
     0.0f,  -0.46f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
     0.0f,   0.0f,  0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 

};

// choose from vertices (eg 0 = top right, 3 = bottom left)
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

    // update
    dvd_update();

    // clear
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // trans
    trans = glm::translate(trans, glm::vec3(dvd_x, dvd_y, 0.0f));

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans)); 
    
    // render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // swap buffers, process events
    glfwSwapBuffers(GlfwHandler.window);
    glfwPollEvents();
   
}

unsigned int handleShaders()
{
    int n;

    CShaderHandler ShaderHandler;
    CUtils Utils;

    memset(sbuffer, 0, sizeof(sbuffer));
    n = Utils.readFile(fragmentShaderPath,sbuffer, BUFFER_SIZE );

    unsigned int fragmentShader = ShaderHandler.compileShader(GL_FRAGMENT_SHADER , sbuffer );

    memset(sbuffer, 0, sizeof(sbuffer));    
    n = Utils.readFile(vertexShaderPath,sbuffer, BUFFER_SIZE );
    

    unsigned int vertexShader  = ShaderHandler.compileShader(GL_VERTEX_SHADER , sbuffer );

    unsigned int shaders[] = {fragmentShader, vertexShader};
    unsigned int shaderProgram = ShaderHandler.makeShaderProgram(shaders);

    return shaderProgram;
}


void dvd_update()
{

    dvd_x += (dvd_speed*dvd_vel_x);
    dvd_y += (dvd_speed*dvd_vel_y);

    if ( (dvd_x + dvd_width) > 1 || dvd_x < -1)
    {
        dvd_vel_x = -dvd_vel_x;
    } else if ( (dvd_y) > 1 || (dvd_y - dvd_height) < -1)
    {
        dvd_vel_y = -dvd_vel_y;
    }

}

int main()
{

    // start -----------------------------------------
    CGlfwHandler GlfwHandler;
    CTextureHandler TextureHandler;
    
    GlfwHandler._glfwInit();
    
    // handle shader related stuff --------------------------

    unsigned int shaderProgram = handleShaders();

    rvStruct _data = processData();  
    unsigned int VAO = _data.data1;
    unsigned int EBO = _data.data2;

    glUseProgram(shaderProgram);

    // handle texture related stuff-------------------------

    TextureHandler.textureLoad(dvdTexturePath, GL_RGBA, false); 

    // render ----------------------------------------------
    glm::mat4 trans = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(GlfwHandler.window))
    {
        Render(GlfwHandler, VAO, shaderProgram, trans);
    }

    // close------------------------------------------------

 
    // deallocate stuffs
    glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    

    glfwTerminate();
    return 0;
}
