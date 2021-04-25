#include "hud/Hud.hpp"

#include "opengl/Framebuffer.hpp"

#include "engine/Window.hpp"
#include "engine/Renderer.hpp"
#include "engine/Application.hpp"
#include "engine/Input.hpp"

#include "lighting/LightManager.hpp"

#include "proceduralPlanet//Planet.hpp"
#include "proceduralPlanet//ShapeSettings.hpp"
#include "proceduralPlanet//ColorSettings.hpp"
#include "proceduralPlanet/noise/NoiseSettings.hpp"
#include "proceduralPlanet/noise/NoiseFilter.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "proceduralPlanet/io/IOManager.hpp"

#include "imgui/imgui_color_gradient.h"

namespace editor
{

using namespace proceduralPlanet;

void Hud::init(Window& window)
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    // Init shared pointers
    _planet = Application::Get().GetPlanet();
    _shape = _planet->shapeSettings();
    _color = _planet->colorSettings();

    // Init windows sizes
    _WIDTH = window.Width();
    _HEIGHT = window.Height();
    _viewportWidth = 0.7 * _WIDTH;
    _viewportHeight = _HEIGHT;
    _settingsWidth = _WIDTH - _viewportWidth;

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

    static bool demo = false;

    if (demo) ImGui::ShowDemoWindow(&demo);
    else
    {

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
        ImGui::Begin("DockSpace Demo", &_dockspaceOpen, window_flags);
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


        if (Application::Get().GetUpdateMode() == UpdateMode::Auto || Application::Get().IsReadyToGenerate())
        {
            Application::Get().GenerateUpdateQueue();
        }

        /* Pop up Windows */
        if (_saveFileOpen) ShowSaveAsWindow();
        if (_newFileOpen) ShowNewSceneWindow();

        /* Permanent Windows */
        if (_settingsOpen) ShowSettingsWindow();
        ShowViewportWindow();
        if (_terminalOpen) ShowLogWindow();
        ShowMenuBar(window);

        ImGui::End(); // Main Window
    }

    // Render ImGUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void  Hud::ShowMenuBar(GLFWwindow* window)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                _newFileOpen = true;
            }

            if (ImGui::BeginMenu("Open scene..."))
            {
                auto paths = IOManager::get().getAllFilesFromFolder("res/scene/");
                for (size_t i = 0; i < paths.size(); i++)
                {
                    if (ImGui::MenuItem(paths[i].c_str()))
                    {
                        _planet->reset();
                        if (!IOManager::get().open(paths[i], _planet))
                        {
                            Application::Get().AppendLog(std::string("Error IO :: cannot open file " + paths[i]).c_str());
                        }
                        else
                        {
                            Application::Get().AppendLog("File has been opened");
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

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
            {
                _dockspaceOpen = false;
                glfwSetWindowShouldClose(window, true);
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Hud::ShowSettingsWindow()
{
    if (ImGui::Begin("Procedural Planets Settings"))
    {
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Planet"))
            {
                ShowUpdateItem();

                if (ImGui::SliderInt("Resolution", &_planet->resolution(), 4, 256))
                {
                    Application::Get().Update(ObserverFlag::RESOLUTION);
                }
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
                if (ImGui::Combo("FaceRenderMask", &(int&)_planet->getFaceRenderMask(), "All\0Top\0Bottom\0Left\0Right\0Front\0Back\0\0"))
                {
                    Application::Get().Update(ObserverFlag::MESH);
                }
                ImGui::PopItemWidth();
                static glm::vec3 globalRot;
                if (ImGui::SliderFloat3("Euler Rotation", (float*)&globalRot, 0.0f, 360.0f))
                {
                    _planet->Rotate(globalRot);
                }

                ImGui::Separator();

                // Changer le seed, les couleurs, et ajoute un epsilon au valeur 
                if (ImGui::Button("Generate Random"))
                {
                    _planet->RandomGenerate();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Noise"))
            {
                ShowUpdateItem();

                static int noiseLayersCount = _shape->noiseLayers().size();
                if (ImGui::SliderInt("Count", &noiseLayersCount, 0, 5))
                {
                    _planet->updateNoiseLayersCount(noiseLayersCount);
                    Application::Get().Update(ObserverFlag::MESH);
                }

                int layerCountNode = 0;
                for (auto& layer : _shape->noiseLayers())
                {
                    std::string titleNode("Noise Layer " + std::to_string(layerCountNode));
                    if (ImGui::TreeNode(titleNode.c_str()))
                    {
                        if (ImGui::Checkbox("Enabled", &layer->enabled()))
                        {
                            Application::Get().Update(ObserverFlag::MESH);
                        }

                        if (ImGui::TreeNode("Noise Settings"))
                        {
                            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
                            if (ImGui::Combo("Filter Type", &(int&)layer->noiseSettings()->GetFilterType(), "Simple\0Rigid\0\0"))
                            {
                                _planet->shapeGenerator()->updateFilterType(layerCountNode);
                                Application::Get().Update(ObserverFlag::MESH);
                            }
                            ImGui::PopItemWidth();


                            if (ImGui::InputInt("Seed", &(int&)_planet->shapeGenerator()->noiseFilter(layerCountNode)->Seed()))
                            {
                                _planet->shapeGenerator()->noiseFilter(layerCountNode)->Reseed();
                                Application::Get().Update(ObserverFlag::MESH);
                            }

                            /*
                            * Display All the Noise Settings
                            */
                            layer->noiseSettings()->Display();


                            ImGui::TreePop();
                        }
                        ImGui::TreePop();
                        ImGui::Separator();
                    }
                    layerCountNode++;
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Material"))
            {
                ShowUpdateItem();

                if (ImGui::TreeNode("Landmass"))
                {
                    if (!_color->GetUseLandmassRamp())
                    {
                        if (ImGui::ColorEdit3("Color", (float*)&(_color->color())))
                        {
                            Application::Get().Update(ObserverFlag::COLOR);
                        }
                    }

                    ImGui::Checkbox("Use Color Ramp", &_color->GetUseLandmassRamp());

                    if (_color->GetUseLandmassRamp())
                    {
                        static ImGradientMark* draggingMark = nullptr;
                        static ImGradientMark* selectedMark = nullptr;
                        ImGui::GradientEditor(&_color->GetGradient(), draggingMark, selectedMark);
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Ocean"))
                {
                    ImGui::Checkbox("Use a different color for ocean", &_color->GetUseOceanColor());

                    if (_color->GetUseOceanColor())
                    {
                        ImGui::SliderFloat("Depth", &_planet->colorSettings()->GetOceanDepth(), 0.0f, 10.0f);
                        ImGui::ColorEdit3("Color", (float*)&_planet->colorSettings()->GetOceanColor());
                    }
                    ImGui::TreePop();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("World"))
            {
                ShowUpdateItem();
                static bool useWorldGradient = true;
                ImGui::ColorEdit3("World Color", (float*)&(Application::Get().GetBackgroundColor()));
                ImGui::SliderFloat("Sun", &LightManager::Get().GetLight()->Intensity(), 0.0f, 1.0f);
                static float ambientGlobal = 0.2f;
                if (ImGui::SliderFloat("Ambient Light", &ambientGlobal, 0.0f, 1.0f))
                {
                    LightManager::Get().GetLight()->SetAmbient(ambientGlobal);
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Overlays"))
            {
                ImGui::Checkbox("Wireframe Mode", &Application::Get().GetEditor().GetWireframePtr());
                ImGui::Checkbox("Show Axis", &Application::Get().GetEditor().GetAxisPtr());
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }



    }
    ImGui::End(); // Settings
}

void Hud::ShowUpdateItem()
{
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
    ImGui::Combo("Update Mode", &(int&)Application::Get().GetUpdateMode(), "Auto\0On Release\0On Generate\0\0");
    ImGui::PopItemWidth();
    if (Application::Get().GetUpdateMode() == UpdateMode::OnGenerate)
    {
        ImGui::SameLine();
        if (ImGui::Button("Generate"))
        {
            Application::Get().SetReadyToGenerate(true);
        }
    }
    ImGui::Separator();
}

void Hud::ShowViewportWindow()
{
    if (ImGui::Begin("Renderer"))
    {
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((ImTextureID)_fbo.id(), wsize, ImVec2(0, 1), ImVec2(1, 0));
        _viewportWidth = wsize.x;
        _viewportHeight = wsize.y;
        _fbo.resize(_viewportWidth, _viewportHeight);
        Renderer::Get().ComputeProjectionMatrix();


        // Loading Wheel
        if (Application::Get().GetLoading())
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground
                | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::SetNextWindowPos(ImVec2(25, _viewportHeight - 50));
            ImGui::SetNextWindowSize(ImVec2(50, 50));
            if (ImGui::Begin("Loading", false, window_flags))
            {
                float frame = Application::Get().GetLastFrameDuration();
                ImGui::Image((ImTextureID)_loadingWheel.GetId(), ImVec2(40, 40),
                    ImVec2(_loadingWheel.GetUV1().x, _loadingWheel.GetUV1().y),
                    ImVec2(_loadingWheel.GetUV2().x, _loadingWheel.GetUV2().y)
                );
                _loadingWheel.NextSprite();
            }
            ImGui::End();
        }

    }
    ImGui::End();
}

void Hud::ShowLogWindow()
{
    Application::Get().DrawLog();
}

void Hud::ShowSaveAsWindow()
{
    ImGui::SetNextWindowPos(ImVec2((_WIDTH / 2.0) - 150, (_HEIGHT / 2.0) - 50));
    ImGui::SetNextWindowSize(ImVec2(300, 100));
    if (ImGui::Begin("Save File"))
    {
        ImGui::Text("Save as ");
        ImGui::InputText(".ini", _bufferSaveLocation, IM_ARRAYSIZE(_bufferSaveLocation));
        if (ImGui::Button("Save"))
        {
            // Error
            if (!IOManager::get().saveAs(std::string("res/scene/" + std::string(_bufferSaveLocation) + ".ini"), _planet))
            {
                Application::Get().AppendLog("Error IO :: cannot save document");
            }
            // Save As Success
            else
            {
                Application::Get().AppendLog("File has been saved");
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

void Hud::ShowNewSceneWindow()
{
    ImGui::SetNextWindowPos(ImVec2((_WIDTH / 2.0) - 150, (_HEIGHT / 2.0) - 50));
    ImGui::SetNextWindowSize(ImVec2(300, 100));
    if (ImGui::Begin("New Scene"))
    {
        ImGui::Text("Save changes before closing?");
        if (ImGui::Button("Save"))
        {
            _saveFileOpen = true;
            _newFileOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Don't Save"))
        {
            _newFileOpen = false;
            IOManager::get().newFile();
            _planet->reset();
            Application::Get().AppendLog("New scene created");
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            _newFileOpen = false;
        }
    }
    ImGui::End();
}


void Hud::setLowSliderSpeed()
{
    _sliderSpeed = 1;
}

void Hud::setDefaultSliderSpeed()
{
    _sliderSpeed = 100;
}

void Hud::ShowSettings()
{
    _settingsOpen = _settingsOpen ? false : true;
}

void Hud::ShowTerminal()
{
    _terminalOpen = _terminalOpen ? false : true;
}

void Hud::saveFile()
{
    // Check whether the file is already save
    if (IOManager::get().currentFileSaved())
    {
        // Check whether the save succeed
        if (!IOManager::get().save(_planet))
        {
            Application::Get().AppendLog("Error IO :: cannot save file ");
        }
        else
        {
            Application::Get().AppendLog("File has been saved");
        }
    }
    // If not, open save as windows
    else
    {
        _saveFileOpen = true;
    }
}

void Hud::newFile()
{
    _newFileOpen = true;
}

void Hud::free()
{
    //Shutdown ImGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete Framebuffer
    _fbo.free();
}

void Hud::bindFbo()
{
    _fbo.bind(_viewportWidth, _viewportHeight);
}

void Hud::unbindFbo()
{
    _fbo.unbind();
}

};  // ns editor

