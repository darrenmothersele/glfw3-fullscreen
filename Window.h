

#ifndef FULLSCREEN_WINDOW_H
#define FULLSCREEN_WINDOW_H

#include <exception>
#include <utility>
#include <GLFW/glfw3.h>
#include <memory>
#include <cstdlib>
#include <iostream>

namespace cy
{
    namespace window
    {

        /**
         * function that wraps up GLFW3 initialization.
         * this MUST be called at the top of all programs
         * before using any windows.
         */
        int init();

        /**
         * Wrapper for GLFW3 terminate and clean up
         */
        void terminate();

        class exception : public std::exception
        {
        public:
            exception(const char*);
            const char * what () const throw ();
        private:
            const char* description;
        };

        class create_failed : public cy::window::exception
        {
        public:
            create_failed();
        };

        /**
         * Used to define window version (OPENGL or OPENGL3)
         * when creating new Window. The default is OPENGL3.
         */
        enum class Type { OPENGL, OPENGL3 };


        /**
         * This is the main window class. In your program
         * you create a single window object, and then run it.
         * Pass your application (subclass of App) as a template
         * parameter to the run method for static polymorphism.
         * Your MyApp class must extend App, and override the
         * call operator.
         * E.g.
         *      class MyApp : public cy::window::App {
         *      public:
         *          void operator()() {
         *              // ...draw stuff here
         *              // you can reference the window
         *              // if you need to, for example:
         *              // window->getFramebufferSize()
         *          }
         *      };
         *      Window window{1920, 1080};
         *      window.run<MyApp>();
         */
        class Window {
        public:
            Window(int width = 640, int height = 480, const char* _t = "Window", bool fullscreen = false, Type type = Type::OPENGL);
            void toggleFullscreen();
            std::pair<int, int> getFramebufferSize();

            template <typename T>
                    void run();

        private:
            GLFWwindow* window;
            const char* title;
            int savedWidth, savedHeight;
            bool fullscreen = false;
            bool window_needs_swapping = false;
            Type type;
            void initialize();
            void refresh();
            int shouldClose();
            void update();
        };

        class App {
        public:
            void setWindowPointer(Window* w) { window = w; }
        protected:
            Window* window;
        };

        template <typename T>
        void Window::run()
        {
            T app;
            app.setWindowPointer(this);
            while (!shouldClose())
            {
                refresh();
                app();
                update();
            }
        }

    }

    template <typename T>
    int bootstrap(int width = 640, int height = 480, const char* _t = "Window",
                  bool fullscreen = false, window::Type type = window::Type::OPENGL)
    {

        if (!cy::window::init())
            return EXIT_FAILURE;

        try {
            cy::window::Window window{width, height, _t, fullscreen, type};
            window.run<T>();
        }
        catch (cy::window::create_failed)
        {
            std::cerr << "Failed to create window!" << std::endl;
        }

        cy::window::terminate();
        return EXIT_SUCCESS;
    }


}


#endif //FULLSCREEN_WINDOW_H
