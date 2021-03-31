#include "engine/Window.hpp"
#include "engine/Application.hpp"

int main(int argc, char** argv)
{
    Window window(argc, argv);

    if (!window.Init())
        return -1;

    /* Main Game Loop */
    mainloop(window);

    glfwTerminate();
    return 0;
}