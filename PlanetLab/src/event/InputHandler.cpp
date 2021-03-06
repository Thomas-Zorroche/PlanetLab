#include "InputHandler.hpp"
#include "Input.hpp"
#include "engine/Camera.hpp"
#include "editor/Application.hpp"
#include "editor/Editor.hpp"

#include "GLFW/glfw3.h"
#include "imgui/imgui_impl_glfw.h"

namespace PlanetLab
{

void InputHandler::SetCallback(GLFWwindow* window, CallbackPtr& callbackPtr)
{
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouseButton_callback);

    glfwSetWindowUserPointer(window, &callbackPtr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetWindowSizeCallback(window, window_size_callback);
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (!Editor::Get().isViewer3DHovered())
        return;

    CallbackPtr* callbackPtr = (CallbackPtr*)glfwGetWindowUserPointer(window);
    auto camera = callbackPtr->_camera;

    if (camera->CanRotate())
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
    if (Editor::Get().isViewer2DHovered())
    {
        Editor::Get().viewer2DZoom(yoffset < 0 ? -0.25 : 0.25);
        return;
    }

    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

    if (!Editor::Get().isViewer3DHovered())
        return;

    CallbackPtr* callbackPtr = (CallbackPtr*)glfwGetWindowUserPointer(window);
    auto camera = callbackPtr->_camera;

    if (yoffset > 0)
        camera->zoom(0.9); // ZOOM IN
    else
        camera->zoom(1.1); // ZOOM OUT
}

/*
* This Function triggers only ONE ACTION
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    if (!Editor::Get().canUseShortcuts())
        return;

    if (key == (int)KeyCode::S && action == GLFW_PRESS)
    {
        if (Input::IsKeyPressed(KeyCode::CTRL))
        {
            Editor::Get().saveFile();
        }
        else
        {
            Editor::Get().toggleDisplaySettings();
        }
    }

    else if (key == (int)KeyCode::T && action == GLFW_PRESS)
    {
        Editor::Get().toggleDisplayLog();
    }

    else if (key == (int)KeyCode::N && action == GLFW_PRESS)
    {
        if (Input::IsKeyPressed(KeyCode::CTRL))
        {
            Editor::Get().newFile();
        }
    }

    else if (key == (int)KeyCode::SHIFT && action == GLFW_PRESS)
    {
        Editor::Get().setLowSliderSpeed();
    }
    else if (key == (int)KeyCode::SHIFT && action == GLFW_RELEASE)
    {
        Editor::Get().setDefaultSliderSpeed();
    }

    // Close Window
    // ===================================================================================================
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // TODO : replace by Ctrl + Q
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
    CallbackPtr* callbackPtr = (CallbackPtr*)glfwGetWindowUserPointer(window);
    auto camera = callbackPtr->_camera;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        camera->SetCanRotate(true);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        camera->SetCanRotate(false);
        if (Application::Get().GetUpdateMode() == UpdateMode::OnRelease)
            Application::Get().SetReadyToGenerate(true);
    }
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    Editor::Get().setWindowSize(width, height);
}


}   // ns PlanetLab

