#include "hud/Hud.hpp"

#include "opengl/Framebuffer.hpp"

#include "engine/Window.hpp"
#include "engine/Renderer.hpp"

#include "planets/Planet.hpp"
#include "planets/ShapeSettings.hpp"
#include "planets/ColorSettings.hpp"
#include "noise/NoiseSettings.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "io/IOManager.hpp"

bool Hud::_wireframeMode = false;

Hud::~Hud()
{
    
}

void Hud::init(GLFWwindow* window, const std::shared_ptr<Planet>& planet, float width, float height)
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    // Init shared pointers
    _planet = planet;
    _shape = planet->shapeSettings();
    _color = planet->colorSettings();

    // Init windows sizes
    _WIDTH = width;
    _HEIGHT = height;
    _viewportWidth = 0.7 * width;
    _viewportHeight = height;
    _settingsWidth = width - _viewportWidth;

    _fbo.resize(_viewportWidth, _viewportHeight);
    
    IOManager::get().setWindowPtr(window);
}

void Hud::draw(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //New Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool demo = false;

    if (demo) ImGui::ShowDemoWindow(&demo);
    else
    {
    static bool dockspaceOpen = true;
    static char bufferSaveLocation[20];
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

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

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

    if (_saveFileOpen)
    {
        ImGui::SetNextWindowPos(ImVec2((_WIDTH / 2.0) - 150, (_HEIGHT / 2.0) - 50));
        ImGui::SetNextWindowSize(ImVec2(300, 100));
        if (ImGui::Begin("Save File"))
        {
            ImGui::Text("Save as ");
            ImGui::InputText(".ini", bufferSaveLocation, IM_ARRAYSIZE(bufferSaveLocation));
            if (ImGui::Button("Save"))
            {
                // Error
                if (!IOManager::get().saveAs(std::string("res/scene/" + std::string(bufferSaveLocation) + ".ini")))
                {
                    _consoleBuffer = "Error IO :: cannot save document";
                }
                // Save As Success
                else
                {
                    _consoleBuffer = "File has been saved";
                }
                _saveFileOpen = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Exit"))
            {
                _saveFileOpen = false;
            }
        }
        ImGui::End();
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                IOManager::get().newFile();
                _planet->reset();
                _consoleBuffer = "New scene created";
            }

            if (ImGui::BeginMenu("Open scene..."))
            {
                auto paths = IOManager::get().getAllFilesFromFolder("res/scene/");
                for (size_t i = 0; i < paths.size(); i++)
                {
                    if (ImGui::MenuItem(paths[i].c_str()))
                    {
                        _planet->reset();
                        if (!IOManager::get().open(paths[i]))
                        {
                            _consoleBuffer = "Error IO :: cannot open file " + paths[i];
                        }
                        else
                        {
                            _consoleBuffer = "File has been opened";
                        }
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                saveFile();
            }

            if (ImGui::MenuItem("Save As..."))
            {
                _saveFileOpen = true;
            }


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

    // Console
    if (ImGui::Begin("Console"))
    {
        ImGui::Text(_consoleBuffer.c_str());
    }
    ImGui::End();

    // Settings
    if (ImGui::Begin("Procedural Planets Settings"))
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (ImGui::CollapsingHeader("Display"))
        {
            ImGui::Checkbox("Wireframe Mode", &_wireframeMode);
        }

        if (ImGui::CollapsingHeader("Planet"))
        {
            if (ImGui::SliderInt("Resolution", &_planet->resolution(), 4, 128))
            {
                _planet->update(ObserverFlag::RESOLUTION);
            }
            if (ImGui::ColorEdit3("Planet Color", (float*)&(_color->color())))
            {
                _planet->update(ObserverFlag::COLOR);
            }
            if (ImGui::SliderFloat("Size", &_shape->planetRadius(), 0.2f, 4.0f))
            {
                _planet->update(ObserverFlag::RADIUS);
            }
        }

        if (ImGui::CollapsingHeader("Noise Layers"))
        {
            static int noiseLayersCount = _shape->noiseLayers().size();
            if (ImGui::SliderInt("Count", &noiseLayersCount, 0, 5))
            {
                _planet->updateNoiseLayersCount(noiseLayersCount);
                _planet->update(ObserverFlag::LAYER);
            }

            int layerCountNode = 0;
            for (auto& layer : _shape->noiseLayers())
            {
                std::string titleNode("Noise Layer " + std::to_string(layerCountNode));
                if (ImGui::TreeNode(titleNode.c_str()))
                {
                    if (ImGui::Checkbox("Enabled", &layer->enabled()))
                    {
                        _planet->update(ObserverFlag::NOISE);
                    }

                    if (ImGui::TreeNode("Noise Settings"))
                    {
                        if (ImGui::SliderFloat("Strength", &layer->noiseSettings()->strength(), 0.0f, 2.0f))
                        {
                            _planet->update(ObserverFlag::NOISE);
                        }
                        if (ImGui::SliderInt("Layers Count", &layer->noiseSettings()->layersCount(), 0, 10))
                        {
                            _planet->update(ObserverFlag::NOISE);
                        }
                        if (ImGui::SliderFloat("Base Roughness", &layer->noiseSettings()->baseRoughness(), 0.0f, 20.0f))
                        {
                            _planet->update(ObserverFlag::NOISE);
                        }
                        if (ImGui::SliderFloat("Roughness", &layer->noiseSettings()->roughness(), 0.0f, 20.0f))
                        {
                            _planet->update(ObserverFlag::NOISE);
                        }
                        if (ImGui::SliderFloat("Persistence", &layer->noiseSettings()->persistence(), 0.0f, 1.0f))
                        {
                            _planet->update(ObserverFlag::NOISE);
                        }
                        if (ImGui::SliderFloat3("Center", (float*)&layer->noiseSettings()->center(), -10.0f, 10.0f))
                        {
                            _planet->update(ObserverFlag::NOISE);
                        }
                        if (ImGui::SliderFloat("Min Value", &layer->noiseSettings()->minValue(), 0.0f, 10.0f))
                        {
                            _planet->update(ObserverFlag::NOISE);
                        }
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                    ImGui::Separator();
                }
                layerCountNode++;
            }
        }


    }
    ImGui::End(); // Settings

    ImGui::End(); // Main Window
    }

    // Render ImGUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Hud::saveFile()
{
    // Check whether the file is already save
    if (IOManager::get().currentFileSaved())
    {
        // Check whether the save succeed
        if (!IOManager::get().save())
        {
            _consoleBuffer = "Error IO :: cannot save file ";
        }
        else
        {
            _consoleBuffer = "File has been saved";
        }
    }
    // If not, open save as windows
    else
    {
        _saveFileOpen = true;
    }
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

