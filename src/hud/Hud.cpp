#include "hud/Hud.hpp"

#include "opengl/Framebuffer.hpp"

#include "engine/Window.hpp"
#include "engine/Renderer.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

bool Hud::_wireframeMode = false;
int Hud::_resolution = 64;
Color Hud::_planetColor;
float Hud::_planetRadius = 1.0f;
float Hud::_noiseStrength = 1.0f;
float Hud::_noiseRoughness = 1.0f;
glm::vec3 Hud::_noiseCenter = glm::vec3(0, 0, 0);


Hud::~Hud()
{
    
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
    _WIDTH = width;
    _HEIGHT = height;
    _viewportWidth = 0.7 * width;
    _viewportHeight = height;
    _settingsWidth = width - _viewportWidth;

    _fbo.resize(_viewportWidth, _viewportHeight);

}

void Hud::draw(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //New Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
     
    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                dockspaceOpen = false;
                glfwSetWindowShouldClose(window, true);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (ImGui::Begin("Renderer"))
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((ImTextureID)_fbo.id(), wsize, ImVec2(0, 1), ImVec2(1, 0));
        _viewportWidth = wsize.x;
        _viewportHeight = wsize.y;
        _fbo.resize(_viewportWidth, _viewportHeight);
        Renderer::Get().ComputeProjectionMatrix();
    }
    ImGui::End();

    // Settings
    if (ImGui::Begin("Procedural Planets Settings"))
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Checkbox("Wireframe Mode", &_wireframeMode);
        ImGui::SliderInt("Resolution", &_resolution, 4, 128);
        ImGui::ColorEdit3("Planet Color", (float*)&_planetColor);
        ImGui::SliderFloat("Size", &_planetRadius, 0.2f, 4.0f);
        ImGui::SliderFloat("Strength", &_noiseStrength, 0.0f, 2.0f);
        ImGui::SliderFloat("Roughness", &_noiseRoughness, 0.0f, 20.0f);
        ImGui::SliderFloat3("Center", (float*)&_noiseCenter, -10.0f, 10.0f);
    }
    ImGui::End(); // Settings

    ImGui::End();

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
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo.id());
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.25f, 0.25f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _viewportWidth, _viewportHeight);
}

void Hud::unbindFbo()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

