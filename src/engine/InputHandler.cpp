#include "engine/InputHandler.hpp"
#include "engine/Camera.hpp"
#include "hud/Hud.hpp"

#include <iostream>
#include "GLFW/glfw3.h"

void InputHandler::ProcessInput(GLFWwindow* window, const std::shared_ptr<Camera>& camera, float deltaTime)
{
    // Close Window
    // ===================================================================================================
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // TODO : replace by Ctrl + Q
    {
        glfwSetWindowShouldClose(window, true);
    }

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
        Hud::get().generateUpdateQueue(true);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        addKey(ActiveKey::CTRL);
        std::cout << "CTRL" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    {
        removeKey(ActiveKey::CTRL);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        addKey(ActiveKey::S);
        if (_canSave && isKeyActive(ActiveKey::CTRL))
        {
            _canSave = false;
            Hud::get().saveFile();
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
    {
        removeKey(ActiveKey::S);
        _canSave = true;
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        addKey(ActiveKey::N);
        if (_canCreateNewFile && isKeyActive(ActiveKey::CTRL))
        {
            _canCreateNewFile = false;
            Hud::get().newFile();
        }
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE)
    {
        removeKey(ActiveKey::N);
        _canCreateNewFile = true;
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

bool InputHandler::isKeyActive(ActiveKey key)
{
    // Test if the key is in the array
    for (const ActiveKey& k : _activeKeys)
    {
        if (k == key) return true;
    }
    return false;
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
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowUserPointer(window, &callbackPtr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    CallbackPtr* callbackPtr = (CallbackPtr*)glfwGetWindowUserPointer(window);
    auto inputHandler = callbackPtr->_inputHandler;
    auto camera = callbackPtr->_camera;

    if (inputHandler->canRotate())
    {
        
        // Get the homogenous position of the camera and pivot point
        glm::vec4 position = glm::vec4(camera->GetPosition(), 1);
        glm::vec4 pivot = glm::vec4(0, 0, 0, 1);
        
        // step 1 : Calculate the amount of rotation given the mouse movement.
        float deltaAngleX = (2 * M_PI / camera->GetWidth()); // a movement from left to right = 2*PI = 360 deg
        float deltaAngleY = (M_PI / camera->GetHeight());  // a movement from top to bottom = PI = 180 deg
        float xAngle = (camera->GetLastX() - xPos) * deltaAngleX;
        float yAngle = (camera->GetLastY() - yPos) * deltaAngleY;

        // Extra step to handle the problem when the camera direction is the same as the up vector
        float cosAngle = glm::dot(camera->GetViewDir(), glm::vec3(0, 1, 0));
        if (cosAngle * glm::sign(yAngle) > 0.99f)
        {
            yAngle = 0;
        }

        // step 2: Rotate the camera around the pivot point on the first axis.
        glm::mat4x4 rotationMatrixX(1.0f);
        rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, glm::vec3(0, 1, 0));
        position = (rotationMatrixX * (position - pivot)) + pivot;

        // step 3: Rotate the camera around the pivot point on the second axis.
        glm::mat4x4 rotationMatrixY(1.0f);
        rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, camera->GetRightVector());
        glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

        // Update the camera view (we keep the same lookat and the same up vector)
        camera->updatePosition(finalPosition);
    }
        // Update the mouse position for the next rotation
        camera->SetLastX(xPos);
        camera->SetLastY(yPos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    CallbackPtr* callbackPtr = (CallbackPtr*)glfwGetWindowUserPointer(window);
    auto camera = callbackPtr->_camera;

    if (yoffset > 0)
        camera->zoom(0.9); // ZOOM IN
    else
        camera->zoom(1.1); // ZOOM OUT
}

