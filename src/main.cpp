#include "engine/Window.hpp"
#include "engine/Application.hpp"

int main(int argc, char** argv)
{
    Application& app = Application(argc, argv);

    if (!app.GetWindow().Init())
        return -1;

    mainloop(app.GetWindow());

    glfwTerminate();
    return 0;
}