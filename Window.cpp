#include "Window.h"

#include <string>

namespace cy
{
    namespace window
    {

        GLFWmonitor* monitor;

        exception::exception(const char * msg) : description(msg) { }
        const char* exception::what() const throw () { return description; }
        create_failed::create_failed() : exception("Failed to create window.") { }

        static void error_callback(int error, const char* description)
        {
            std::string msg{std::to_string(error) + ": " + std::string(description)};
            throw exception(msg.c_str());
        }

        void key_callback(GLFWwindow* w, int key, int s, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                if (key == GLFW_KEY_F && mods == GLFW_MOD_SUPER)
                {
                    static_cast<Window*>(glfwGetWindowUserPointer(w))->toggleFullscreen();
                }
            }
        }

        int init()
        {
            glfwSetErrorCallback(error_callback);
            int status = glfwInit();
            if (status)
            {
                monitor = glfwGetPrimaryMonitor();
            }
            return status;
        }
        void terminate()
        {
            glfwTerminate();
        }


        GLFWwindow* open_window(GLFWmonitor* monitor, int width, int height, const char* title, bool fullscreen, Type type)
        {
            if (type == Type::OPENGL3)
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
            }

            if (fullscreen)
            {
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
                return glfwCreateWindow(mode->width, mode->height, title, monitor, NULL);
            }
            return glfwCreateWindow(width, height, title, NULL, NULL);
        }

        Window::Window(int width, int height, const char* _t, bool _f, Type _type)
                : fullscreen(_f), title(_t), savedWidth(width), savedHeight(height), type(_type)
        {
            window = open_window(monitor, width, height, title, fullscreen, type);
            if (!window)
            {
                glfwTerminate();
                throw create_failed();
            }
            initialize();
        }

        void Window::initialize()
        {
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1);
            glfwSetWindowUserPointer(window, this);
            glfwSetKeyCallback(window, key_callback);
        }

        void Window::refresh()
        {
            if (window_needs_swapping)
            {
                window_needs_swapping = false;
                if (fullscreen)
                {
                    // switching to fullscreen, so save current window size
                    glfwGetWindowSize(window, &savedWidth, &savedHeight);
                }
                glfwDestroyWindow(window);
                window = open_window(monitor, savedWidth, savedHeight, title, fullscreen, type);
                if (!window)
                {
                    glfwTerminate();
                    throw create_failed();
                }
                initialize();
            }
            glfwPollEvents();
        }
        void Window::update() {
            glfwSwapBuffers(window);
        }

        int Window::shouldClose() {
            return glfwWindowShouldClose(window);
        }

        std::pair<int, int> Window::getFramebufferSize() {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            return std::pair<int, int>(width, height);
        }

        void Window::toggleFullscreen()
        {
            fullscreen = !fullscreen;
            window_needs_swapping = true;
        }

    }
}