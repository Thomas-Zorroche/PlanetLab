#include "ui/Interface.hpp"

#include "engine/opengl/Framebuffer.hpp"
#include "engine/Window.hpp"
#include "engine/Renderer.hpp"
#include "engine/lighting/LightManager.hpp"
#include "editor/Application.hpp"
#include "event/Input.hpp"

#include "ceres/Planet.hpp"
#include "ceres/ShapeSettings.hpp"
#include "ceres/ColorSettings.hpp"
#include "ceres/noise/NoiseSettings.hpp"
#include "ceres/noise/NoiseFilter.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_color_gradient.h"

#include "io/IOManager.hpp"


namespace PlanetLab
{

using namespace Ceres;

using namespace PlanetLab;


void Interface::init(Window& window)
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
    _observer = std::make_unique<UiObserver>(_planet->getPlanetSubject());
    _shape = _planet->getShapeSettings();
    _color = _planet->getColorSettings();

    // Init windows sizes
    _WIDTH = window.Width();
    _HEIGHT = window.Height();
    _viewportWidth = 0.7 * _WIDTH;
    _viewportHeight = _HEIGHT;
    _settingsWidth = _WIDTH - _viewportWidth;

    _fbo.resize(_viewportWidth, _viewportHeight);

    updateStatistics();

    // Setup theme
    setDarkThemeMode();

    // Setup fonts
    io.Fonts->AddFontFromFileTTF("res/fonts/OpenSans/OpenSans-Bold.ttf", 18.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 5.0f;
}


void Interface::draw(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //New Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool demo = false;
    if (demo)
    {
        ImGui::ShowDemoWindow(&demo);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        return;
    }

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
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSize = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSize;

    // Display launch screen
    if (_launchScreenOpen) 
        ShowLaunchScreen();

    /* Pop up Windows */
    if (_saveFileOpen) 
        ShowSaveAsWindow();
    if (_newFileOpen) 
        ShowNewSceneWindow();

    /* Permanent Windows */
    if (_settingsOpen)
        ShowSettingsWindow();
    if (_terminalOpen) 
        ShowLogWindow();
    ShowViewportWindow();
    ShowMenuBar(window);

    ImGui::End(); // Main Window

    // Render ImGUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

template <typename UIFonction>
static void drawParameter(const std::string& name, UIFonction uiFonction)
{
    if (!name.empty())
    {
        float posX = (ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() * 0.4) - ImGui::CalcTextSize(name.c_str()).x
            - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
        if (posX > ImGui::GetCursorPosX())
            ImGui::SetCursorPosX(posX);

        ImGui::AlignTextToFramePadding();
        ImGui::Text(name.c_str()); ImGui::SameLine();
    }

    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.4);
    
    uiFonction();

    ImGui::PopItemWidth();
}


bool Interface::ShowLaunchScreen()
{
    ImGuiWindowFlags window_flags_launch_screen = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove;

    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];
    
    ImGui::SetNextWindowPos(ImVec2((_WIDTH - _launchScreen.Width() * 0.5) * 0.5, (_HEIGHT - _launchScreen.Height()) * 0.5));
    ImGui::SetNextWindowSize(ImVec2(1280 * 0.5, 480));
    bool clickInside = false;
    ImGuiStyle& style = ImGui::GetStyle();
    float baseWindowRounding = style.WindowRounding;
    style.WindowRounding = 6.0f;
    style.WindowPadding = ImVec2(0.0f, 0.0f);
    style.WindowBorderSize = 0.0f;
    ImGui::SetNextWindowBgAlpha(0.85f);
    if (ImGui::Begin("LaunchScreen", &_launchScreenOpen, window_flags_launch_screen))
    {
        // Launch screen image
        ImGui::Image((ImTextureID)_launchScreen.Id(), ImVec2(1280 * 0.5, 480 * 0.5), ImVec2(0, 0), ImVec2(1, 1));
        
        // Files Window
        float windowWidth = ImGui::GetWindowContentRegionWidth();
        ImVec2 toolbarSize(windowWidth, 200);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 10));
        if (ImGui::BeginChild("Recent Files", toolbarSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding))
        {
            ImGui::PushFont(boldFont);
            ImGui::Text("New File");
            ImGui::PopFont();

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0, 0, 0, 0));
            if (ImGui::Button("General"))
            {
                _launchScreenOpen = false;
                IOManager::get().newFile();
                _planet->reset();
                Application::Get().AppendLog("New scene created");
                clickInside = true;
            }
            ImGui::PopStyleColor();

            //ImGui::Separator();

            ImGui::PushFont(boldFont);
            ImGui::Text("Recent Files");
            ImGui::PopFont();

            auto paths = IOManager::get().getAllFilesFromFolder("res/scene/");
            for (size_t i = 0; i < paths.size(); i++)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(0, 0, 0, 0) );
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                if (ImGui::Button(paths[i].c_str()))
                {
                    _launchScreenOpen = false;
                    if (!IOManager::get().open(paths[i], _planet))
                    {
                        Application::Get().AppendLog(std::string("Error IO :: cannot open file " + paths[i]).c_str());
                    }
                    else
                    {
                        _noiseSettingsParameters.clear();
                        const auto layersCount = _planet->getShapeSettings()->getNoiseLayers().size();
                        for (size_t i = 0; i < layersCount; i++)
                            _noiseSettingsParameters.push_back(_planet->getShapeSettings()->getNoiseLayer(i)->getNoiseSettings());

                        Application::Get().AppendLog("File has been opened");
                    }
                }
                ImGui::PopStyleColor();
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImGui::Separator();

        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionWidth() * 0.5) - 0.5 * ImGui::CalcTextSize("  Close  ").x);
        if (ImGui::BeginChild("Close"))
        {
            if (ImGui::Button("Close")) _launchScreenOpen = false;
        }
        ImGui::EndChild();

    }
    ImGui::End();

    style.WindowRounding = baseWindowRounding;
    return clickInside;
}

void Interface::ShowMenuBar(GLFWwindow* window)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                if (IOManager::get().getUnsavedValues())
                    _newFileOpen = true;
                else
                {
                    if (_launchScreenOpen) 
                        _launchScreenOpen = false;

                    IOManager::get().newFile();
                    _planet->reset();
                    Application::Get().AppendLog("New scene created");
                }
            }

            if (ImGui::BeginMenu("Open scene..."))
            {
                auto paths = IOManager::get().getAllFilesFromFolder("res/scene/");
                for (size_t i = 0; i < paths.size(); i++)
                {
                    if (ImGui::MenuItem(paths[i].c_str()))
                    {
                        if (!IOManager::get().open(paths[i], _planet))
                        {
                            Application::Get().AppendLog(std::string("Error IO :: cannot open file " + paths[i]).c_str());
                        }
                        else
                        {
                            if (_launchScreenOpen)
                                _launchScreenOpen = false;

                            _noiseSettingsParameters.clear();
                            const auto layersCount = _planet->getShapeSettings()->getNoiseLayers().size();
                            for (size_t i = 0; i < layersCount; i++)
                                _noiseSettingsParameters.push_back(_planet->getShapeSettings()->getNoiseLayer(i)->getNoiseSettings());

                            Application::Get().AppendLog("File has been opened");
                        }
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();
            if (!IOManager::get().getUnsavedValues())
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.2, 0.2, 0.2, 0.8 });
                ImGui::MenuItem("Save", "Ctrl+S");
                ImGui::PopStyleColor();
            }
            else
            {
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                    saveFile();
            }

            if (ImGui::MenuItem("Save As..."))
                _saveFileOpen = true;

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

void Interface::drawUpdateModeItem()
{
    drawParameter("Update Mode", []()
    {
        ImGui::Combo("##Update Mode", &(int&)Application::Get().GetUpdateMode(), "Auto\0On Release\0On Generate\0\0");
    });

    drawParameter("", []()
    {
        if (Application::Get().GetUpdateMode() == UpdateMode::OnGenerate)
        {
            if (ImGui::Button("Generate"))
                Application::Get().SetReadyToGenerate(true);
        }
        ImGui::Separator();
    });
}

void Interface::ShowSettingsWindow()
{
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    if (ImGui::Begin("Settings"))
    {
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_FittingPolicyResizeDown;
        if (_planet->isVisible() && ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            // Planet Settings
            // ***********************************************************************
            ImGui::PushFont(boldFont);
            if (ImGui::BeginTabItem("Planet"))
            {
                ImGui::PushFont(nullptr);

                drawUpdateModeItem();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.9f, 0.3f, 0.1f, 1.0f });
                drawParameter("", [&planet = _planet]()
                {
                    ImGui::Text("Warning: depending on your computer,\nresolution above 128 could slow down the application.");
                });
                ImGui::PopStyleColor();

                drawParameter("Resolution", [&planet = _planet]()
                {
                    if (ImGui::SliderInt("##Resolution", &planet->getResolution(), 4, 256))
                    {
                        Application::Get().Update(ObserverFlag::RESOLUTION);
                    }
                });

                drawParameter("FaceRenderMask", [&planet = _planet]()
                {
                    if (ImGui::Combo("##FaceRenderMask", &(int&)planet->getFaceRenderMask(), "All\0Top\0Bottom\0Left\0Right\0Front\0Back\0\0"))
                    {
                        Application::Get().Update(ObserverFlag::MESH);
                        Application::Get().Update(ObserverFlag::FACERENDERMASK);
                    }
                });

                drawParameter("Rotation Speed", [&planet = _planet]()
                {
                    ImGui::SliderFloat("##Rotation Speed", &planet->getRotationSpeed(), 0.0f, 0.5f);
                });

                ImGui::Separator();

                drawParameter("", [&planet = _planet]()
                {
                    if (ImGui::Button("Generate Random"))
                    {
                        planet->generateRandomPlanet();
                    }
                });


                ImGui::EndTabItem();
                ImGui::PopFont();
            }
            ImGui::PopFont();

            
            // Noise Settings
            // ***********************************************************************
            ImGui::PushFont(boldFont);
            if (ImGui::BeginTabItem("Noise"))
            {
                ImGui::PushFont(nullptr);
                drawUpdateModeItem();

                drawParameter("Count", [&planet = _planet]()
                {
                    int noiseLayersCount = planet->getShapeSettings()->getNoiseLayers().size();
                    if (ImGui::SliderInt("##Count", &noiseLayersCount, 0, 5))
                    {
                        Interface::Get().updateNoiseLayersCount(noiseLayersCount);
                        Application::Get().Update(ObserverFlag::MESH);
                    }
                });

                int layerCountNode = 0;
                for (auto& layer : _shape->getNoiseLayers())
                {
                    ImGui::Separator();

                    std::string titleNode = "Noise Layer " + std::to_string(layerCountNode + 1);
                    if (ImGui::TreeNode(titleNode.c_str()))
                    {
                        drawParameter("", [&planet = _planet, &layer]()
                        {
                            if (ImGui::Checkbox("Enabled", &layer->isEnabled()))
                            {
                                Application::Get().Update(ObserverFlag::MESH);
                            }
                        });

                        drawParameter("FilterType", [&planet = _planet, &layer, &layerCountNode]()
                        {
                            if (ImGui::Combo("##Filter Type", &(int&)layer->getNoiseSettings()->filterType, "Simple\0Rigid\0\0"))
                            {
                                planet->getShapeGenerator()->updateFilterType(layerCountNode);
                                Application::Get().Update(ObserverFlag::MESH);
                            }
                        });

                        drawParameter("Seed", [&planet = _planet, &layer, &layerCountNode]()
                        {
                            if (ImGui::InputInt("##Seed", &(int&)planet->getShapeGenerator()->getNoiseFilter(layerCountNode)->getSeed()))
                            {
                                planet->getShapeGenerator()->getNoiseFilter(layerCountNode)->reseed();
                                Application::Get().Update(ObserverFlag::MESH);
                            }
                        });

                        // Display filter noise settings
                        _noiseSettingsParameters[layerCountNode].display();

                        ImGui::TreePop();

                    }
                    layerCountNode++;
                    
                }
                ImGui::EndTabItem();
                ImGui::PopFont();
            }
            ImGui::PopFont();


            // Material Settings
            // ***********************************************************************
            ImGui::PushFont(boldFont);
            if (ImGui::BeginTabItem("Material"))
            {
                ImGui::PushFont(nullptr);
                drawUpdateModeItem();

                if (ImGui::TreeNode("Landmass"))
                {
                    if (!_color->getUseLandmassRamp())
                    {
                        drawParameter("Color", [&color = _color]()
                        {
                            if (ImGui::ColorEdit3("##Color", (float*)&(color->getLandmassColor())))
                                Application::Get().Update(ObserverFlag::COLOR);
                        });
                    }

                    drawParameter("", [&color = _color]()
                    {
                        ImGui::Checkbox("Use Color Ramp", &color->getUseLandmassRamp());
                    });

                    if (_color->getUseLandmassRamp())
                    {
                        static ImGradientMark* draggingMark = nullptr;
                        static ImGradientMark* selectedMark = nullptr;
                        ImGui::GradientEditor(&_color->getGradient(), draggingMark, selectedMark);
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Ocean"))
                {
                    drawParameter("", [&color = _color]()
                    {
                        ImGui::Checkbox("Use a different color for ocean", &color->getUseOceanColor());
                    });

                    if (_color->getUseOceanColor())
                    {
                        drawParameter("Depth", [&color = _color]()
                        {
                            ImGui::SliderFloat("##Depth", &color->getOceanDepth(), 0.0f, 10.0f);
                        });

                        drawParameter("Color", [&color = _color]()
                        {
                            ImGui::ColorEdit3("##Color", (float*)&color->getOceanColor());
                        });
                    }
                    ImGui::TreePop();
                }

                ImGui::EndTabItem();
                ImGui::PopFont();
            }
            ImGui::PopFont();

            // World Settings
            // ***********************************************************************
            ImGui::PushFont(boldFont);
            if (ImGui::BeginTabItem("World"))
            {
                ImGui::PushFont(nullptr);
                drawUpdateModeItem();

                drawParameter("Sun", []()
                {
                    ImGui::SliderFloat("##Sun", &LightManager::Get().GetLight()->Intensity(), 0.0f, 1.0f);
                });

                drawParameter("Ambient Light", []()
                {
                    static float ambientGlobal = 0.2f;
                    if (ImGui::SliderFloat("Ambient Light##", &ambientGlobal, 0.0f, 1.0f))
                    {
                        LightManager::Get().GetLight()->SetAmbient(ambientGlobal);
                    }                
                });

                drawParameter("", [&color = _color]()
                {
                    ImGui::Checkbox("Use Skybox", &Application::Get().GetEditor().isSkyboxDisplayed());
                });

                ImGui::EndTabItem();
                ImGui::PopFont();
            }
            ImGui::PopFont();

            // Overlays Settings
            // ***********************************************************************
            ImGui::PushFont(boldFont);
            if (ImGui::BeginTabItem("Overlays"))
            {
                ImGui::PushFont(nullptr);

                drawParameter("", []()
                {
                    ImGui::Checkbox("Wireframe Mode", &Application::Get().GetEditor().IsWireframeVisible());
                });

                drawParameter("", []()
                {
                    ImGui::Checkbox("Show Axis", &Application::Get().GetEditor().IsAxisVisible());
                });

                drawParameter("", []()
                {
                    ImGui::Checkbox("Show Statistics", &Application::Get().GetEditor().IsStatsVisible());
                });

                ImGui::EndTabItem();
                ImGui::PopFont();
            }
            ImGui::PopFont();

            ImGui::EndTabBar();
        }

    }
    ImGui::End(); // Settings
}



void Interface::ShowViewportWindow()
{
    if (ImGui::Begin("Renderer"))
    {
        // 3D Viewport
        ImVec2 wsize = ImGui::GetContentRegionAvail();
        ImGui::Image((ImTextureID)_fbo.getTextureId(), wsize, ImVec2(0, 1), ImVec2(1, 0));
        _viewportWidth = wsize.x;
        _viewportHeight = wsize.y;
        _fbo.resize(_viewportWidth, _viewportHeight);
        Renderer::Get().ComputeProjectionMatrix();


        // Statistics
        if (Application::Get().GetEditor().IsStatsVisible() && _planet->isVisible())
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground
                | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::SetNextWindowPos(ImVec2(40, 80));
            ImGui::SetNextWindowSize(ImVec2(150, 100));
            if (ImGui::Begin("Statistics", false, window_flags))
            {
                ImGui::Text("Vertices    %s", _statistics.verticesCount.c_str());
                ImGui::Text("Faces        %s", _statistics.facesCount.c_str());
                ImGui::Text("Triangles  %s", _statistics.trianglesCount.c_str());
                ImGui::Text("Fps            %.1f", ImGui::GetIO().Framerate);
            }
            ImGui::End();
        }

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

void Interface::ShowLogWindow()
{
    Application::Get().DrawLog();
}

void Interface::ShowSaveAsWindow()
{
    ImGui::SetNextWindowPos(ImVec2((_WIDTH * 0.5) - 150, (_HEIGHT * 0.5) - 60));
    ImGui::SetNextWindowSize(ImVec2(300, 120));
    if (ImGui::Begin("Save File"))
    {
        ImGui::Text("Save as ");
        ImGui::InputText(".ini", _bufferSaveLocation, IM_ARRAYSIZE(_bufferSaveLocation));
        if (ImGui::Button("Save"))
        {
            if (std::string(_bufferSaveLocation).empty())
            {
                // Error empty buffer
                Application::Get().AppendLog("Filename cannot be empty");
            }
            else if (!IOManager::get().saveAs(std::string("res/scene/" + std::string(_bufferSaveLocation) + ".ini"), _planet))
            {
                // Error Cannot Save
                Application::Get().AppendLog("Error IO :: cannot save document");
            }
            else
            {
                // Save As Success
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

void Interface::ShowNewSceneWindow()
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


void Interface::setLowSliderSpeed()
{
    _sliderSpeed = 1;
}

void Interface::setDefaultSliderSpeed()
{
    _sliderSpeed = 100;
}

void Interface::ShowSettings()
{
    _settingsOpen = _settingsOpen && !_saveFileOpen ? false : true;
}

void Interface::ShowTerminal()
{
    _terminalOpen = _terminalOpen && !_saveFileOpen ? false : true;
}

void Interface::saveFile()
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

void Interface::newFile()
{
    _newFileOpen = true;
}

void Interface::updateNoiseLayersCount(int noiseLayersCountUpdated)
{
    int layersCountDifference = noiseLayersCountUpdated - _planet->getShapeSettings()->getNoiseLayers().size();

    if (layersCountDifference == 0)
        return;

    if (layersCountDifference > 0)
    {
        for (size_t i = 0; i < layersCountDifference; i++)
            _planet->addNoiseLayer();

        // Add a noiseSettingsParamater
        _noiseSettingsParameters.push_back(NoiseSettingsParameters(_planet->getShapeSettings()->getLastLayer()->getNoiseSettings()));
    }
    else
    {
        for (size_t i = 0; i < abs(layersCountDifference); i++)
            _planet->removeLastNoiseLayer();
        
        // Remove the noiseSettingsParamater linked with last layer to be removed
        _noiseSettingsParameters.pop_back();
    }
}



void Interface::free()
{
    //Shutdown ImGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete Framebuffer
    _fbo.free();
}

void Interface::bindFbo()
{
    _fbo.bind(_viewportWidth, _viewportHeight);
}

void Interface::unbindFbo()
{
    _fbo.unbind();
}

void Interface::setWindowSize(int width, int height)
{
    _WIDTH = width;
    _HEIGHT = height;
}


void Interface::updateStatistics()
{
    prettyPrintNumber(_planet->getVerticesCount(), _statistics.verticesCount);
    prettyPrintNumber(_planet->getFacesCount(), _statistics.facesCount);
    prettyPrintNumber(_planet->getFacesCount() * 2, _statistics.trianglesCount);
}

void Interface::onResolutionUpdate(int resolution)
{
    updateStatistics();
}


// Turn 1451862 into 1 451 862
// TODO: stock in variable and call this onVerticesChange
void prettyPrintNumber(int number, std::string& str)
{
    str = std::to_string(number);
    str.reserve(str.length() + str.length() / 3);

    for (int i = 0, j = 3 - str.length() % 3; i < str.length(); ++i, ++j)
        if (i != 0 && j % 3 == 0)
            str.insert(i++, 1, ' ');
}

void Interface::setDarkThemeMode()
{
    // NOTE: change colors with ImGui demo windows
    
    // Setup main colors
    const ImVec4 lightGreen = ImVec4{ 0, 0.8, 0.478, 1.0f };
    const ImVec4 green = ImVec4{ 0.149, 0.509, 0.415, 1.0f };

    const ImVec4 darkPurple = ImVec4{ 0.117, 0.109, 0.137, 1.0f };
    const ImVec4 purple = ImVec4{ 0.196, 0.172, 0.227, 1.0f };
    const ImVec4 lightPurple = ImVec4{ 0.305, 0.262, 0.356, 1.0f };

    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = darkPurple;

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.380, 0.772, 0.733, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = green;
    colors[ImGuiCol_ButtonHovered] = lightGreen;
    colors[ImGuiCol_ButtonActive] = lightGreen;

    // Frame Components BG
    colors[ImGuiCol_FrameBg] = purple;
    colors[ImGuiCol_FrameBgHovered] = lightPurple;
    colors[ImGuiCol_FrameBgActive] = lightPurple;

    // Tabs
    colors[ImGuiCol_Tab] = green;
    colors[ImGuiCol_TabHovered] = lightGreen;
    colors[ImGuiCol_TabActive] = lightGreen;
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Slider
    colors[ImGuiCol_SliderGrab] = green;
    colors[ImGuiCol_SliderGrabActive] = lightGreen;

    // Checkbox
    colors[ImGuiCol_CheckMark] = green;
}

};  // ns editor

