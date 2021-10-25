#include "SettingsPanel.hpp"
#include "editor/Application.hpp"
#include "editor/Editor.hpp"
#include "engine/lighting/LightManager.hpp"
#include "editor/Parameter.hpp"

#include "Ceres/noise/NoiseLayer.hpp"

#include "imgui/imgui.h"

namespace PlanetLab
{

using namespace Ceres;
using namespace PlanetLab;


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
    }, "When does planet updates have to be executed:\n * Auto: directly after user changes\n * On Release: after user release click on any parameter\n * On Generate: after click on generate button.");

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
    }, "Number of edge subdivisions per Terrain Face.");

    drawParameter("FaceRenderMask", [&planet = _planet]()
    {
        if (ImGui::Combo("##FaceRenderMask", &(int&)planet->getFaceRenderMask(), "All\0Top\0Bottom\0Left\0Right\0Front\0Back\0\0"))
        {
            Application::Get().Update(ObserverFlag::MESH);
            Application::Get().Update(ObserverFlag::FACERENDERMASK);
        }
    }, "Whether to render one Terrain Face of the planet, or the entierty.");

    drawParameter("Rotation Speed", [&planet = _planet]()
    {
        ImGui::SliderFloat("##Rotation Speed", &planet->getRotationSpeed(), 0.0f, 0.5f);
    }, "Rotation speed around Y axis.");

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
    }, "Generate random values for colors, noise seed and noise location.");
}

void SettingsPanel::displayNoisePanel()
{
    drawParameter("Count", [this]()
    {
        int noiseLayersCount = _planet->getShapeGenerator()->getNoiseLayers().size();
        if (ImGui::SliderInt("##Count", &noiseLayersCount, 0, 5))
        {
            updateNoiseLayersCount(noiseLayersCount);
            Application::Get().Update(ObserverFlag::MESH);
        }
    }, "Number of noise layers");

    int layerIndex = 0;
    for (auto& uiNoiseLayer : _uiNoiseLayers)
    {
        if (uiNoiseLayer.draw(Editor::Get().getSelectedLayerId()))
            Editor::Get().setSelectedLayerId(layerIndex);
        ++layerIndex;
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
            }, "Landmass color.");
        }

        drawParameter("", [&color = colorSettings]()
        {
            ImGui::Checkbox("Use Color Ramp", &color->getUseLandmassRamp());
        }, "Use multiple colors for landmass.");

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
            }, "Use unscaled elevation to retrieve depth under water.");

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
        if (ImGui::SliderFloat("##Ambient Light", &ambientGlobal, 0.0f, 1.0f))
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
    int layersCountDifference = noiseLayersCountUpdated - _planet->getShapeGenerator()->getNoiseLayers().size();

    if (layersCountDifference == 0)
        return;

    if (layersCountDifference > 0)
    {
        for (size_t i = 0; i < layersCountDifference; i++)
        {
            // Add Ceres noise layer and layer
            auto layer = _planet->addNoiseLayer();

            // Add PlanetLab corresponding ui noise layer
            _uiNoiseLayers.push_back(UINoiseLayer(layer, _planet, _sliderSpeed));
        }
    }
    else
    {
        for (size_t i = 0; i < abs(layersCountDifference); i++)
        {
            // Remove Ceres noise layer and layer
            _planet->removeLastNoiseLayer();

            // Remove the ui noise layer linked with last layer to be removed
            _uiNoiseLayers.pop_back();
        }
    }
}

void SettingsPanel::clearNoiseLayers()
{ 
    _uiNoiseLayers.clear();
}

void SettingsPanel::addNoiseLayer(std::shared_ptr<Ceres::NoiseLayer> noiselayer)
{ 
    _uiNoiseLayers.push_back(UINoiseLayer(noiselayer, _planet, _sliderSpeed));
}

} // ns PlanetLab