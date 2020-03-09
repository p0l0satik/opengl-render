#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include "Bitmap.h"
// #include "shader.h"

#include <iostream>
// #include <vector>

#define WIDTH 500
#define HEIGHT 600

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


struct Figure
{
    GLfloat* vertices;
    GLuint* indices;
    
    GLuint VBO, VAO;
    Figure(GLfloat* vertices){
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        this->vertices = vertices;
        bindVertAttributes(VBO, VAO, this->vertices);
        //init vertices and indices
    }

    void bindVertAttributes(GLuint VBO, GLuint VAO, GLfloat* vertices){
        // 1. Привязываем VAO
        glBindVertexArray(VAO);
        // 2. Копируем наш массив вершин в буфер для OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, VBO); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
        // 3. Устанавливаем указатели на вершинные атрибуты 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0); 
        //4. Отвязываем VAO
        glBindVertexArray(0); 
    }

};


int main(){
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT); //set window size

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };  

    Figure triangle = Figure(vertices);
    // // 1. Привязываем VAO
    // glBindVertexArray(VAO);
    // // 2. Копируем наш массив вершин в буфер для OpenGL
    // glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    // // 3. Устанавливаем указатели на вершинные атрибуты 
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    // glEnableVertexAttribArray(0); 
    // //4. Отвязываем VAO
    // glBindVertexArray(0); 


    //created shader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const char* basic_v_addr = "shader/basic_v.glsl";
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    // compiled shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }



    const char *fragmentShaderSource = "#version 330 core \n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    GLuint shaderProgram;
    shaderProgram = glCreateProgram();


    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

       
        glUseProgram(shaderProgram);
        glBindVertexArray(triangle.VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }


    glfwTerminate();
    return 0;
}