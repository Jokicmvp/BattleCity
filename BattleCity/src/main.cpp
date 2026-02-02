#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer/ShaderProgram.h"

GLfloat point[] = {
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLuint points_index[] = {
    0, 2, 3,
    0, 1, 2
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
};

const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);"
"}";


int g_WIndowSizeX = 640;
int g_WIndowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    g_WIndowSizeX = width;
    g_WIndowSizeY = height;
    glViewport(0, 0, g_WIndowSizeX, g_WIndowSizeY);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) 
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }
        
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(g_WIndowSizeX, g_WIndowSizeY, "Battle city", nullptr, nullptr);
    if (!window)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) 
    {
        std::cout << "Can't load GLAD!" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    glClearColor(0, 1, 1, 1);

    std::string vertexShader(vertex_shader);
    std::string fragmentShader(fragment_shader);
    Renderer::ShaderProgram shaderProgram(vertex_shader, fragment_shader);
    if (!shaderProgram.isCompiled())
    {
        std::cout << "Can't create shader program!" << std::endl;
    }
    
    GLuint vao,points_vbo, colors_vbo, ebo;

    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &points_vbo);
    glGenBuffers(1, &colors_vbo);
    glGenBuffers(1, &ebo);    

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);   
    
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(points_index), points_index, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}