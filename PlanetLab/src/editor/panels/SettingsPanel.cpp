#include "SettingsPanel.hpp"
#include "editor/Application.hpp"
#include "editor/Editor.hpp"
#include "engine/lighting/LightManager.hpp"

#include "Ceres/noise/NoiseFilter.hpp"

#include "imgui/imgui.h"

namespace PlanetLab
{

using namespace Ceres;
using namespace PlanetLab;

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

        // Hover description
        //const std::string& desc 
        //if (!desc.empty() && ImGui::IsItemHovered())
        //{
        //    ImGui::BeginTooltip();
        //    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        //    ImGui::TextUnformatted(desc.c_str());
        //    ImGui::PopTextWrapPos();
        //    ImGui::EndTooltip();
        //}
    }

    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.4);

    uiFonction();

    ImGui::PopItemWidth();
}

SettingsPanel::SettingsPanel(std::shared_ptr<Ceres::Planet> planet)
    : _planet(planet)
{
}

void SettingsPanel::draw()
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

                displayPlanetPanel();

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

                displayNoisePanel();
                
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

                displayMaterialPanel();

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

                displayWorldPanel();

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

                displayOverlaysPanel();

                ImGui::EndTabItem();
                ImGui::PopFont();
            }
            ImGui::PopFont();

            ImGui::EndTabBar();
        }

    }
    ImGui::End(); // Settings
}

void SettingsPanel::drawUpdateModeItem()
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

void SettingsPanel::displayPlanetPanel()
{
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

    drawParameter("", [&planet = _planet]()
    {
        ImGui::Checkbox("Hide Surface", &planet->isSurfaceHidden());
    });

    ImGui::Separator();

    drawParameter("", [&planet = _planet]()
    {
        if (ImGui::Button("Generate Random"))
        {
            planet->generateRandomPlanet();
        }
    });
}

void SettingsPanel::displayNoisePanel()
{
    drawParameter("Count", [this]()
        {
            int noiseLayersCount = _planet->getShapeSettings()->getNoiseLayers().size();
            if (ImGui::SliderInt("##Count", &noiseLayersCount, 0, 5))
            {
                updateNoiseLayersCount(noiseLayersCount);
                Application::Get().Update(ObserverFlag::MESH);
            }
        });

    int layerCountNode = 0;
    for (auto& layer : _planet->getShapeSettings()->getNoiseLayers())
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

            drawParameter("FilterType", [&planet = _planet, &layer, &layerCountNode, &noiseSettingsParameters = _noiseSettingsParameters]()
            {
                if (ImGui::Combo("##Filter Type", &(int&)layer->getNoiseSettings()->filterType, "Simple\0Rigid\0\0"))
                {
                    planet->getShapeGenerator()->updateFilterType(layerCountNode);
                    noiseSettingsParameters[layerCountNode].setFilterType(layer->getNoiseSettings()->filterType);
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
            _noiseSettingsParameters[layerCountNode].display(_sliderSpeed);

            ImGui::TreePop();

        }
        layerCountNode++;

    }
}

void SettingsPanel::displayMaterialPanel()
{
    auto colorSettings = _planet->getColorSettings();

    if (ImGui::TreeNode("Landmass"))
    {
        if (!colorSettings->getUseLandmassRamp())
        {
            drawParameter("Color", [&color = colorSettings]()
            {
                if (ImGui::ColorEdit3("##Color", (float*)&(color->getLandmassColor())))
                    Application::Get().Update(ObserverFlag::COLOR);
            });
        }

        drawParameter("", [&color = colorSettings]()
        {
            ImGui::Checkbox("Use Color Ramp", &color->getUseLandmassRamp());
        });

        if (colorSettings->getUseLandmassRamp())
        {
            static ImGradientMark* draggingMark = nullptr;
            static ImGradientMark* selectedMark = nullptr;
            ImGui::GradientEditor(&colorSettings->getGradient(), draggingMark, selectedMark);
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Ocean"))
    {
        drawParameter("", [&color = colorSettings]()
        {
            ImGui::Checkbox("Use a different color for ocean", &color->getUseOceanColor());
        });

        if (colorSettings->getUseOceanColor())
        {
            drawParameter("Depth", [&color = colorSettings]()
            {
                ImGui::SliderFloat("##Depth", &color->getOceanDepth(), 0.0f, 10.0f);
            });

            drawParameter("Color", [&color = colorSettings]()
            {
                ImGui::ColorEdit3("##Color", (float*)&color->getOceanColor());
            });
        }
        ImGui::TreePop();
    }
}

void SettingsPanel::displayWorldPanel()
{
    auto colorSettings = _planet->getColorSettings();

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

    drawParameter("", [&color = colorSettings, &editorSettings = Editor::Get().getEditorSettings()]()
    {
        ImGui::Checkbox("Use Skybox", &editorSettings->isSkyboxDisplayed());
    });
}

void SettingsPanel::displayOverlaysPanel()
{
    drawParameter("", [&editorSettings = Editor::Get().getEditorSettings()]()
    {
        ImGui::Checkbox("Display Wireframe", &editorSettings->IsWireframeVisible());
    });

    drawParameter("", [&editorSettings = Editor::Get().getEditorSettings()]()
    {
        ImGui::Checkbox("Display Axis", &editorSettings->IsAxisVisible());
    });

    drawParameter("", [&editorSettings = Editor::Get().getEditorSettings()]()
    {
        ImGui::Checkbox("Display Statistics", &editorSettings->IsStatsVisible());
    });

    drawParameter("", [&editorSettings = Editor::Get().getEditorSettings()]()
    {
        ImGui::Checkbox("Display Points", &editorSettings->isPointsDisplayed());

        if (editorSettings->isPointsDisplayed())
        {
        drawParameter("Point Size", [&editorSettings = Editor::Get().getEditorSettings()]()
        {
            ImGui::SliderInt("Point Size##", &editorSettings->getSizePoints(), 0, 10);
        });
        }
    });
}


void SettingsPanel::updateNoiseLayersCount(int noiseLayersCountUpdated)
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

} // ns PlanetLab