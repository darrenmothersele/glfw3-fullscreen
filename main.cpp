
#include "Window.h"

using namespace std;

class OpenGLDemoApp : public cy::window::App
{
public:
    void operator()()
    {
        std::pair<int,int> fboSize = window->getFramebufferSize();
        float ratio{fboSize.first / (float) fboSize.second};
        glViewport(0, 0, fboSize.first, fboSize.second);
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
    }
};


int main()
{
    return cy::bootstrap<OpenGLDemoApp>(640, 480, "My Example", false, cy::window::Type::OPENGL);
}
