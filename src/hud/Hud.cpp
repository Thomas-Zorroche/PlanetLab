#include "hud/Hud.hpp"

#include "opengl/Framebuffer.hpp"

#include "engine/Camera.hpp"
#include "engine/Window.hpp"
#include "engine/Renderer.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

bool Hud::_wireframeMode = false;
int Hud::_resolution = 64;
Color Hud::_planetColor;
float Hud::_planetRadius = 1.0f;

Hud::~Hud()
{
    //glDeleteFramebuffers(1, &_fboViewport);
}

void Hud::init(GLFWwindow* window, float width, float height)
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    // Init windows sizes
    _viewportWidth = 0.7 * width;
    _viewportHeight = height;
    _settingsWidth = width - _viewportWidth;

    // Create 3D Viewport Framebuffer
    _fboViewport = Framebuffer(_viewportWidth, _viewportHeight).id();
}

void Hud::draw(const std::shared_ptr<Camera>& camera) const
{
    //New Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 3D Viewport
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(_viewportWidth, _viewportHeight));
        ImGui::Begin("Viewport");
        {
            ImGui::BeginChild("Renderer");
            ImVec2 wsize = ImGui::GetWindowSize();
            ImGui::Image((ImTextureID)_fboViewport, wsize, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();
    }

    // Settings
    {
        ImGui::SetNextWindowPos(ImVec2(_viewportWidth, 0));
        ImGui::SetNextWindowSize(ImVec2(_settingsWidth, _viewportHeight));
        ImGui::Begin("Procedural Planets Settings");
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Checkbox("Wireframe Mode", &_wireframeMode);
            ImGui::SliderInt("Resolution", &_resolution, 4, 128);
            ImGui::ColorEdit3("Planet Color", (float*)&_planetColor);
            ImGui::SliderFloat("Size", &_planetRadius, 0.2f, 4.0f);
        }
        ImGui::End();
    }

    // Render ImGUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Hud::free()
{
    //Shutdown ImGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Hud::bindFbo()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fboViewport);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.25f, 0.25f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _viewportWidth, _viewportHeight);
}

void Hud::unbindFbo()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}