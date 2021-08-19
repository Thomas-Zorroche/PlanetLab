#include "editor/Editor.hpp"

#include "engine/opengl/Framebuffer.hpp"
#include "engine/Window.hpp"
#include "engine/Renderer.hpp"
#include "editor/Application.hpp"
#include "event/Input.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_color_gradient.h"

#include "io/IOManager.hpp"


namespace PlanetLab
{

using namespace Ceres;

using namespace PlanetLab;


void Editor::init(Window& window)
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
    _settingsPanel.setPlanet(_planet);
    _viewer3DPanel.setPlanet(_planet);
    _viewer2DPanel.setPlanet(_planet);
    _observer = std::make_unique<UiObserver>(_planet->getPlanetSubject());

    // Init windows sizes
    _WIDTH = window.Width();
    _HEIGHT = window.Height();
    _settingsWidth = _WIDTH - _viewer3DPanel.getViewportWidth();

    _viewer3DPanel.updateStatistics();

    // Setup Theme
    _editorSettings->setDarkThemeMode();

    // Setup fonts
    io.Fonts->AddFontFromFileTTF("res/fonts/OpenSans/OpenSans-Bold.ttf", 18.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 4.0f;
    style.GrabRounding = 3.0f;
}


void Editor::draw(GLFWwindow* window)
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


    /* Pop up Windows */
    if (_launchScreenOpen)  displayLaunchScreen();
    if (_displaySaveAsPopup) displaySaveAsPopup();
    if (_saveBeforeCloseParams.display)  displaySaveBeforeClosePopup();

    /* Permanent Windows */
    if (_settingsOpen) _settingsPanel.draw();
    if (_logOpen) displayLog();
    _viewer3DPanel.draw();
    if (_viewer2DOpen) _viewer2DPanel.draw();
    displayMenuBar(window);

    ImGui::End(); // Main Window
}

void Editor::render()
{
    // Render ImGUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Editor::displayLaunchScreen()
{
    ImGuiWindowFlags window_flags_launch_screen = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove;

    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];
    
    ImGui::SetNextWindowPos(ImVec2((_WIDTH - _launchScreen.Width() * 0.5) * 0.5, (_HEIGHT - _launchScreen.Height()) * 0.5));
    ImGui::SetNextWindowSize(ImVec2(1280 * 0.5, 480));
    bool clickInside = false;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
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
                newFile();
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
                        _settingsPanel.clearNoiseSettings();
                        const auto layersCount = _planet->getShapeSettings()->getNoiseLayers().size();
                        for (size_t i = 0; i < layersCount; i++)
                            _settingsPanel.addNoiseSettings(_planet->getShapeSettings()->getNoiseLayer(i)->getNoiseSettings());

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
    ImGui::PopStyleVar(3);

    return clickInside;
}

template <typename UIFunction>
static void displayWindowMenuItem(const std::string& label, bool open, UIFunction uiFunction)
{
    bool closeNext = false;
    if (!open)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.5, 0.5, 0.5, 0.8 });
        closeNext = true;
    }
    if (ImGui::MenuItem(label.c_str()))
    {
        uiFunction();
    }
    if (closeNext)
    {
        ImGui::PopStyleColor();
        closeNext = false;
    }
}

void Editor::displayMenuBar(GLFWwindow* window)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                if (_launchScreenOpen)
                    _launchScreenOpen = false;

                // If any unsaved values, ask for save
                if (IOManager::get().areUnsavedValues())
                {
                    _saveBeforeCloseParams = SaveBeforeCloseParams(true, "new");
                }
                else
                {
                    newFile();
                }
            }

            if (ImGui::BeginMenu("Open scene..."))
            {
                auto paths = IOManager::get().getAllFilesFromFolder("res/scene/");
                for (size_t i = 0; i < paths.size(); i++)
                {
                    if (ImGui::MenuItem(paths[i].c_str()))
                    {
                        if (_launchScreenOpen)
                            _launchScreenOpen = false;

                        // If any unsaved values, ask for save
                        if (IOManager::get().areUnsavedValues())
                        {
                            _saveBeforeCloseParams = SaveBeforeCloseParams(true, "open", paths[i]);
                        }
                        else
                        {
                            openFile(paths[i]);
                        }
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();
            if (!IOManager::get().areUnsavedValues())
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.2, 0.2, 0.2, 0.8 });
                ImGui::MenuItem("Save", "Ctrl+S");
                ImGui::PopStyleColor();
            }
            else
            {
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                    saveFile();
                }
            }

            if (ImGui::MenuItem("Save As..."))
            {
                _displaySaveAsPopup = true;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
            {
                _dockspaceOpen = false;
                glfwSetWindowShouldClose(window, true);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            displayWindowMenuItem("Settings",  _settingsOpen, [this]() { toggleDisplaySettings(); });
            displayWindowMenuItem("Log",       _logOpen,      [this]() { toggleDisplayLog(); });
            displayWindowMenuItem("Viewer 2D", _viewer2DOpen, [this]() { toggleDisplayViewer2D(); });
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void Editor::displayLog()
{
    Application::Get().DrawLog();
}

void Editor::displaySaveAsPopup()
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
            _displaySaveAsPopup = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit"))
        {
            _displaySaveAsPopup = false;
        }
    }
    ImGui::End();
}

void Editor::displaySaveBeforeClosePopup()
{
    ImGui::SetNextWindowPos(ImVec2((_WIDTH / 2.0) - 150, (_HEIGHT / 2.0) - 50));
    ImGui::SetNextWindowSize(ImVec2(300, 100));
    if (ImGui::Begin("New Scene"))
    {
        ImGui::Text("Save changes before closing?");
        if (ImGui::Button("Save"))
        {
            _saveBeforeCloseParams.display = false;
            _displaySaveAsPopup = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Don't Save"))
        {
            _saveBeforeCloseParams.display = false;
            if (_saveBeforeCloseParams.action == "new")
            {
                newFile();
            }
            if (_saveBeforeCloseParams.action == "open")
            {
                openFile(_saveBeforeCloseParams.pathToOpen);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            _saveBeforeCloseParams.display = false;
        }
    }
    ImGui::End();
}

void Editor::toggleDisplaySettings()
{
    _settingsOpen = _settingsOpen && !_displaySaveAsPopup ? false : true;
}

void Editor::toggleDisplayLog()
{
    _logOpen = _logOpen && !_displaySaveAsPopup ? false : true;
}

void Editor::toggleDisplayViewer2D()
{
    _viewer2DOpen = _viewer2DOpen && !_displaySaveAsPopup ? false : true;
}

void Editor::saveFile()
{
    if (!IOManager::get().areUnsavedValues())
        return;

    if (!IOManager::get().isFileOnDisk())
        _saveBeforeCloseParams.display = true;
     
    Application::Get().AppendLog(IOManager::get().save(_planet) ? "File has been saved" : "Error IO :: cannot save file ");
}

void Editor::newFile()
{
    IOManager::get().newFile();
    _settingsPanel.clearNoiseSettings();
    _planet->reset();
    Application::Get().AppendLog("New scene created");
}

void Editor::openFile(const std::string& filePath)
{
    if (IOManager::get().open(filePath, _planet))
    {
        _settingsPanel.clearNoiseSettings();
        const auto layersCount = _planet->getShapeSettings()->getNoiseLayers().size();
        for (size_t i = 0; i < layersCount; i++)
            _settingsPanel.addNoiseSettings(_planet->getShapeSettings()->getNoiseLayer(i)->getNoiseSettings());

        Application::Get().AppendLog("File has been opened");
    }
    else
    {
        Application::Get().AppendLog("Error opening file.");
    }
}


void Editor::free()
{
    //Shutdown ImGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete Framebuffer
    _viewer3DPanel.freeFbo();
}

void Editor::setWindowSize(int width, int height)
{
    _WIDTH = width;
    _HEIGHT = height;
}

void Editor::onResolutionUpdate(int resolution)
{
    _viewer3DPanel.updateStatistics();
}

void Editor::onMeshUpdate()
{
    _viewer2DPanel.updateTexture();
}


};  // ns editor

