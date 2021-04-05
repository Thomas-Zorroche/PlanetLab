#include "engine/Application.hpp"
#include "engine/Renderer.hpp"
#include "engine/Camera.hpp"
#include "engine/Scene.hpp"
#include "engine/InputHandler.hpp"
#include "engine/ResourceManager.hpp"
#include "hud/Hud.hpp"


void mainloop(Window& windowObject)
{
    GLFWwindow* window = windowObject.WindowPtr();

    // Load all the 
    ResourceManager::Get().LoadAllShaders();

    Scene scene;
    Hud::get().init(window, windowObject.Width(), windowObject.Height());

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
        inputHandler->ProcessInput(window, camera, deltaTime);

        // View Matrix
        Renderer::Get().ComputeViewMatrix();

        glClearColor(0.25f, 0.25f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene here
        Hud::get().bindFbo();
        scene.Draw();

        // Render Hud
        Hud::get().unbindFbo();
        Hud::get().draw(window);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    scene.Free();
}