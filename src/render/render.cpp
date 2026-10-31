#include "render.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstddef>

namespace cedis{
    Render::Render(){

    }
    Render::~Render(){

    }


    int Render::initWindow(){
        // Init library
        if(!glfwInit()){
            return -1;
        }

        m_window = glfwCreateWindow(640, 480, "Spooky Amdus Window", NULL, NULL);
        if(!m_window){
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(m_window);

        Render::loop();
        return 0;
    }

    int Render::loop(){
        while(!glfwWindowShouldClose(m_window)){
            // Render
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap front and back buffers
            glfwSwapBuffers(m_window);

            // Poll for processed events
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
}