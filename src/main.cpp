#include "engine/Window.hpp"
#include "engine/Application.hpp"

int main(int argc, char** argv)
{
    editor::Application& app = editor::Application(argc, argv);

    if (!app.GetWindow().Init())
        return -1;

    editor::mainloop(app.GetWindow());

    glfwTerminate();
    return 0;
}