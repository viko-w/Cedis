#pragma once

#include <GLFW/glfw3.h>

namespace cedis {
    class Render{
        public:
        Render();
        ~Render();

        int initWindow();
        int loop();

        private:
        GLFWwindow* m_window;

    };
}