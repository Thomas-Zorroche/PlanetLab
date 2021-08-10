#include "editor/Application.hpp"
#include "editor/EditorSettings.hpp"

#include "engine/ResourceManager.hpp"
#include "engine/Renderer.hpp"
#include "engine/Camera.hpp"
#include "engine/Scene.hpp"

#include "event/InputHandler.hpp"

#include "editor/Editor.hpp"

#include "io/IOManager.hpp"

#include "Ceres/Planet.hpp"

namespace PlanetLab
{

void mainloop(Window& windowObject)
{
    GLFWwindow* window = windowObject.GetNativeWindow();

    // Load all the 
    ResourceManager::Get().LoadAllShaders();

    Scene scene;
    Editor::Get().init(windowObject);

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
        Editor::Get().bindFbo();
        scene.Draw(Editor::Get().viewportHeight());
        Editor::Get().unbindFbo();

        // Render UI
        Editor::Get().draw(window);

        if (Application::Get().GetUpdateMode() == UpdateMode::Auto || Application::Get().IsReadyToGenerate())
        {
            Application::Get().GenerateUpdateQueue();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    Editor::Get().free();
    scene.Free();
}

/*
* Application Class
*/

Application* Application::s_instance = nullptr;

Application::Application(int argc, char** argv)
{
    s_instance = this;
    
    PlanetLab::Log::init(argc, argv);
    _window = std::make_unique<Window>(argc, argv);
}

void Application::GenerateUpdateQueue(bool onRelease)
{
    if (!_updatesQueue.empty()/* && (onRelease && _updateMode == UpdateMode::OnRelease) || !onRelease*/)
    {
        IOManager::get().setUnsavedValues();
        for (const auto& flag : _updatesQueue)
        {
            _planet->update(flag);
        }
        _updatesQueue.clear();
    }
    _loading = false;
    _readyToGenerate = false;
}

void Application::Update(Ceres::ObserverFlag flag)
{
    _loading = true;
    AddUpdateIntoQueue(flag);
}

void Application::AddUpdateIntoQueue(Ceres::ObserverFlag flag)
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

void Application::updateTitle(const std::string& newTitle)
{
    glfwSetWindowTitle(_window->GetNativeWindow(), newTitle.c_str());
}


} // ns editor


