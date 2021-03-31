#include "engine/InputHandler.hpp"
#include "engine/Camera.hpp"

#include <iostream>
#include "GLFW/glfw3.h"

void InputHandler::ProcessInput(GLFWwindow* window, const std::shared_ptr<Camera>& camera, float deltaTime)
{
    // Close Window
    // ===================================================================================================
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Sprint 
    // ===================================================================================================
    float boostSprint = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        boostSprint = 2.0f; 

    // ===================================================================================================
    Movement(window, camera, deltaTime * boostSprint);

    // ===================================================================================================
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && _ActiveKey != ActiveKey::C) // C Qwerty = C Azerty
    {
        _ActiveKey = ActiveKey::C;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && _ActiveKey == ActiveKey::C)
    {
        _ActiveKey = ActiveKey::NONE;
    }

    // ===================================================================================================
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && _ActiveKey != ActiveKey::A) // Q Qwerty = A Azerty
    {
        _ActiveKey = ActiveKey::A;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE && _ActiveKey == ActiveKey::A)
    {
        _ActiveKey = ActiveKey::NONE;

    }

}

void InputHandler::Movement(GLFWwindow* window, const std::shared_ptr<Camera>& camera, float deltaTime) {
    // Movement Inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)        // W Qwerty = Z Azerty
        camera->Move(deltaTime, DIRCAM::FRONT);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)   // S Qwerty = S Azerty
        camera->Move(-deltaTime, DIRCAM::FRONT);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)   // A Qwerty = Q Azerty
        camera->Move(deltaTime, DIRCAM::LEFT);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)   // D Qwerty = D Azerty
        camera->Move(-deltaTime, DIRCAM::LEFT);
}


void InputHandler::SetCallback(GLFWwindow* window, CallbackPtr& callbackPtr)
{
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowUserPointer(window, &callbackPtr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //CallbackPtr* callbackPtr = (CallbackPtr*)glfwGetWindowUserPointer(window);
    //auto camera = callbackPtr->_camera;

    //float xoffset = xpos - camera->GetLastX();
    //float yoffset = ypos - camera->GetLastY();
    //camera->SetLastX(xpos);
    //camera->SetLastY(ypos);

    //xoffset *= camera->GetSensitivity();
    //yoffset *= camera->GetSensitivity();

    //camera->rotateLeft(xoffset);
    //camera->rotateUp(yoffset);
}

