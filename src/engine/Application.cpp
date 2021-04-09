#include "engine/Application.hpp"
#include "engine/Renderer.hpp"
#include "engine/Camera.hpp"
#include "engine/Scene.hpp"
#include "engine/InputHandler.hpp"
#include "engine/ResourceManager.hpp"
#include "hud/Hud.hpp"
#include "planets/Planet.hpp"


void mainloop(Window& windowObject)
{
    GLFWwindow* window = windowObject.WindowPtr();

    // Load all the 
    ResourceManager::Get().LoadAllShaders();

    Scene scene;
    std::shared_ptr<Application> application = std::make_shared<Application>(scene.planet());
    Hud::get().init(window, application, windowObject.Width(), windowObject.Height());

    auto camera = std::make_shared<Camera>();
    Renderer::Get().SetCamera(camera);

    // Initialize GLFW Callbacks and Inputs
    auto inputHandler = std::make_shared<InputHandler>();
    CallbackPtr callbackPtr(camera, inputHandler);
    inputHandler->SetCallback(window, callbackPtr);

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    glEnable(GL_DEPTH_TEST);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle Inputs
        inputHandler->ProcessInput(window, application, camera, deltaTime);

        // View Matrix
        Renderer::Get().ComputeViewMatrix();

        glClearColor(0.25f, 0.25f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene here
        Hud::get().bindFbo();
        scene.Draw(application->IsWireframeMode());

        // Render Hud
        Hud::get().unbindFbo();
        Hud::get().draw(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    Hud::get().free();
    scene.Free();
}

/*
* Application Class
*/

Application::Application(const std::shared_ptr<Planet>& planet)
    : _planet(planet)
{

}

bool Application::IsWireframeMode() const
{
    return _wireframeMode;
}

void Application::SetWireframeMode(bool mode)
{
    _wireframeMode = mode;
}

bool& Application::GetWireframeModePtr()
{
    return _wireframeMode;
}

void Application::GenerateUpdateQueue(bool onRelease)
{
    if (!_updatesQueue.empty() && (onRelease && _updateMode == UpdateMode::OnRelease) || !onRelease)
    {
        for (const auto& flag : _updatesQueue)
        {
            _planet->update(flag);
        }
        _updatesQueue.clear();
    }
}

void Application::Update(ObserverFlag flag)
{
    _updateMode == UpdateMode::Auto ? _planet->update(flag) : AddUpdateIntoQueue(flag);
}

void Application::AddUpdateIntoQueue(ObserverFlag flag)
{
    bool alreadyIn = false;
    for (const auto& f : _updatesQueue)
    {
        if (flag == f) alreadyIn = true;
    }

    if (!alreadyIn)
    {
        _updatesQueue.push_back(flag);
    }
}

std::shared_ptr<Planet> Application::PlanetPtr()
{ 
    return _planet; 
}

