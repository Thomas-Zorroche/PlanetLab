#include "engine/Application.hpp"
#include "engine/Renderer.hpp"
#include "engine/Camera.hpp"
#include "engine/Scene.hpp"
#include "engine/InputHandler.hpp"
#include "engine/ResourceManager.hpp"
#include "hud/Hud.hpp"
#include "planets/Planet.hpp"
#include "io/IOManager.hpp"
#include "engine/ProceduralEditor.hpp"

namespace editor
{

using namespace proceduralPlanet;

void mainloop(Window& windowObject)
{
    GLFWwindow* window = windowObject.GetNativeWindow();
    IOManager::get().setWindowPtr(window);

    // Load all the 
    ResourceManager::Get().LoadAllShaders();

    Scene scene;
    Hud::Get().init(windowObject);

    auto camera = std::make_shared<Camera>();
    Renderer::Get().SetCamera(camera);

    // Initialize GLFW Callbacks and Inputs
    auto inputHandler = std::make_shared<InputHandler>();
    CallbackPtr callbackPtr(camera);
    inputHandler->SetCallback(window, callbackPtr);

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - Application::Get().GetLastFrameDuration();
        Application::Get().SetLastFrameDuration(currentFrame);

        // View Matrix
        Renderer::Get().ComputeViewMatrix();

        Application::Get().ClearColor();

        // Render scene here
        Hud::Get().bindFbo();
        scene.Draw(Hud::Get().viewportHeight());

        // Render Hud
        Hud::Get().unbindFbo();
        Hud::Get().draw(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    Hud::Get().free();
    scene.Free();
}

/*
* Application Class
*/

Application* Application::s_instance = nullptr;

Application::Application(int argc, char** argv)
{
    s_instance = this;
    _window = std::make_unique<Window>(argc, argv);
    _editor = std::make_unique<ProceduralEditor>();
}

void Application::GenerateUpdateQueue(bool onRelease)
{
    if (!_updatesQueue.empty()/* && (onRelease && _updateMode == UpdateMode::OnRelease) || !onRelease*/)
    {
        for (const auto& flag : _updatesQueue)
        {
            _planet->update(flag);
        }
        _updatesQueue.clear();
    }
    _loading = false;
    _readyToGenerate = false;
}

void Application::Update(ObserverFlag flag)
{
    _loading = true;
    AddUpdateIntoQueue(flag);
}

void Application::AddUpdateIntoQueue(ObserverFlag flag)
{
    bool alreadyIn = false;
    for (const auto& f : _updatesQueue)
    {
        if (flag == f)
        {
            alreadyIn = true;
            break;
        }
    }

    if (!alreadyIn)
    {
        _updatesQueue.push_back(flag);
    }
}

void Application::ClearColor() const
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // ns editor


