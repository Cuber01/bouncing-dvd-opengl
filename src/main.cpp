#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstring>
#include <fstream>
using std::ifstream;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// set up vertex data (and buffer(s)) and configure vertex attributes
float vertices[] = {

    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

};

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow* window;
int success;
char infoLog[512];

const char* fragmentShaderPath = "./res/fragment_shader.fs";
const char* vertexShaderPath = "./res/vertex_shader.vs";

#define BUFFER_SIZE 2000;

char sbuffer[BUFFER_SIZE];

int _glfwInit()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    return 0;

}

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

char data[2000];//todo: poprawic

int readFile( const char* fileName, char* buffer, unsigned int maxFileSize )
{
    unsigned int n;
    ifstream ifs(fileName);
    std::cout << std::endl << "Read FILE: " << fileName << std::endl;
    // ifs.open(fileName);
    ifs.read( buffer, maxFileSize );
    n = ifs.gcount();
    ifs.close();

    return n;
}

void printFile( const char* data, unsigned int n ){
    std::cout << std::endl << "--- Readed FILE Start: ----------------------------------------------" << std::endl;
    for( int i=0; i< n; i++ ){
        std::cout << data[i];
    }
    std::cout << std::endl << "--- Readed FILE End: ----------------------------------------------" << std::endl;    
}



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



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


int main()
{
    int n;

    _glfwInit();
    
    memset(sbuffer, 0, sizeof(sbuffer));
    n = readFile(fragmentShaderPath,sbuffer, BUFFER_SIZE );
    printFile( sbuffer,n);
    std::cout << std::endl << "--- Compile: ----------------------------------------------" << std::endl;    
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER , sbuffer );

    memset(sbuffer, 0, sizeof(sbuffer));    
    n = readFile(vertexShaderPath,sbuffer, BUFFER_SIZE );
    printFile( sbuffer,n);        
    std::cout << std::endl << "--- Compile: ----------------------------------------------" << std::endl;        
    unsigned int vertexShader  = compileShader(GL_VERTEX_SHADER , sbuffer );

    unsigned int shaders[] = {fragmentShader, vertexShader};
    unsigned int shaderProgram = makeShaderProgram(shaders);

    unsigned int VAO = processVerticesData();


    glUseProgram(shaderProgram);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
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
