#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if(!glfwInit()){
        std::cerr << "FAILED TO INITIALIZE GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800,600,"OPENGL TEST",NULL,NULL);
    if(!window){
        std::cerr << "FAILED TO OPEN WINDOW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cerr << "FAILED TO INITIALIZE GLEW\n";
        return -1;
    }

    glClearColor(0.2f,0.3f,0.4f,1.0f); // Set background color

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
