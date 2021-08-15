#include "engine/Window.hpp"
#include "editor/Application.hpp"


int main(int argc, char** argv)
{
    PlanetLab::Application app(argc, argv);
    
    if (!app.GetWindow().Init())
        return 1;

    mainloop(app.GetWindow());

    glfwTerminate();
    return 0;
}

