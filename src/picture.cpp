#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include "Bitmap.h"
// #include "shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#define WIDTH 500
#define HEIGHT 600

using shaderPaths = std::map<std::string, std::string>;


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


struct Shader{
    std::string source;
    uint id;

    Shader(std::string source){
        this->source = source;
    }
    void compile(){
        std::string pretext = readFromFile();
        const char* text = pretext.c_str();
        //compilation
        glShaderSource(id, 1, &text, NULL);
        glCompileShader(id);
        checkCompile();
    }

    void checkCompile(){
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    std::string readFromFile(){
        std::fstream shader_file(source);
        std::stringstream shader_stream;

        shader_stream << shader_file.rdbuf();

        shader_file.close();

        return shader_stream.str();

    }
};

struct VertexShader: public Shader{

    VertexShader(std::string source):Shader(source){
        id = glCreateShader(GL_VERTEX_SHADER);  
        compile();

    }
};

struct FragmentShader: public Shader{

    FragmentShader(std::string source):Shader(source){
        id = glCreateShader(GL_FRAGMENT_SHADER);  
        compile();

    }
};

struct ShaderProgram{
    uint id;
    ShaderProgram(std::vector<uint>& shaders){
        id = glCreateProgram();

        for (auto shader : shaders){
            attachShader(shader);
        }

        glLinkProgram(id);

        for (auto shader : shaders){
            glDeleteShader(shader);
        }
    }
    void attachShader(uint shaderId){
        glAttachShader(id, shaderId);

        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        
        if (!success) {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER_PROGRAMM:ATTACHEMENT_FAILED\n" << infoLog << std::endl;
        }
    }
};


struct Figure{
    float* vertices;
    uint vert_size;
    // GLuint* indices;  
    uint VBO, VAO;

    

    ShaderProgram* shaderProgram;
    

    Figure(float* vertices, uint& vert_size, shaderPaths& shader_paths){
       
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        this->vertices = vertices;
        this->vert_size = vert_size;
        bindVertAttributes();

        shaderProgram = makeShaderProgram(shader_paths);
    }

    ShaderProgram* makeShaderProgram(shaderPaths& shader_paths){
        std::vector<uint> shaderIds;
        VertexShader vertexShader = VertexShader(shader_paths["vertex"]);
        FragmentShader fragmentShader = FragmentShader(shader_paths["fragment"]);
        shaderIds.push_back(vertexShader.id);
        shaderIds.push_back(fragmentShader.id);
        return new ShaderProgram(shaderIds);
    }

    void render(){
        glUseProgram(shaderProgram->id);
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

    void bindVertAttributes(){
        // 1. Привязываем VAO
        glBindVertexArray(VAO);
        // 2. Копируем наш массив вершин в буфер для OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, VBO); 
        glBufferData(GL_ARRAY_BUFFER, vert_size * sizeof(vertices), vertices, GL_STATIC_DRAW); 
        // 3. Устанавливаем указатели на вершинные атрибуты 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
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
     float vertices[] ={
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };  
    
    uint vert_size = 9;

    shaderPaths shader_paths;
    shader_paths["vertex"] =  "./shaders/basic_v.glsl";
    shader_paths["fragment"] =  "./shaders/basic_f.glsl";
                 
    Figure triangle = Figure(vertices, vert_size, shader_paths);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        triangle.render();
       
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }


    glfwTerminate();
    return 0;
}