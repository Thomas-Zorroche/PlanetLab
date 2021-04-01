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

    // ===================================================================================================
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) 
    {
        addKey(ActiveKey::ALT);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
    {
        removeKey(ActiveKey::ALT);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        addKey(ActiveKey::MOUSE_LEFT);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        removeKey(ActiveKey::MOUSE_LEFT);
    }

}

void InputHandler::addKey(ActiveKey key)
{
    // Test if the key is already in the array
    bool inside = false;
    for (const ActiveKey& k : _activeKeys)
    {
        if (k == key)
        {
            inside = true; 
            break;
        }
    }
    
    if (!inside)
    {
        _activeKeys.push_back(key);
    }
}

void InputHandler::removeKey(ActiveKey key)
{
    // Test if the key is in the array
    int index = -1, i = 0;
    for (const ActiveKey& k : _activeKeys)
    {
        if (k == key)
        {
            index = i;
            break;
        }
        i++;
    }

    if (index != -1)
    {
        _activeKeys.erase(_activeKeys.begin() + index);
    }
}

bool InputHandler::canRotate() const
{
    // Test if ALT and MOUSE_LEFT are in the Active Keys
    int canRotate = 0;
    for (const ActiveKey& k : _activeKeys)
    {
        if (k == ActiveKey::ALT) canRotate++;
        if (k == ActiveKey::MOUSE_LEFT) canRotate++;
    }
    return canRotate == 2 ? true : false;
}


void InputHandler::SetCallback(GLFWwindow* window, CallbackPtr& callbackPtr)
{
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowUserPointer(window, &callbackPtr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    CallbackPtr* callbackPtr = (CallbackPtr*)glfwGetWindowUserPointer(window);
    auto inputHandler = callbackPtr->_inputHandler;

    if (inputHandler->canRotate())
    {
        auto camera = callbackPtr->_camera;

        float xoffset = xpos - camera->GetLastX();
        float yoffset = ypos - camera->GetLastY();
        camera->SetLastX(xpos);
        camera->SetLastY(ypos);

        xoffset *= camera->GetSensitivity();
        yoffset *= camera->GetSensitivity();

        camera->rotateLeft(xoffset);
        camera->rotateUp(yoffset);
    }
}

