#include "UINoiseLayer.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_groupPanel.hpp"

#include "editor/Application.hpp"
#include "editor/Parameter.hpp"
#include "Editor.hpp"

#include "Ceres/noise/NoiseLayer.hpp"

#include "imgui/IconsMaterialDesignIcons.h"

namespace PlanetLab
{

UINoiseLayer::UINoiseLayer(std::shared_ptr<Ceres::NoiseLayer> noiseLayer,
        std::shared_ptr<Ceres::Planet> planet, float& sliderSpeed)
    : _noiseLayer(noiseLayer), 
	_id(noiseLayer->getId()), 
    _name("Noise Layer" + std::to_string((_id + 1))),
    _planet(planet),
    _sliderSpeed(sliderSpeed),
    _noiseSettingsParameters(noiseLayer->getNoiseSettings(), _id)
{
}

template <typename UIFonction>
static void drawIconButton(const std::string& label, const std::string& desc, bool state, UIFonction uiFonction)
{
    ImGui::PushStyleColor(ImGuiCol_Text, state ? ImVec4{ 1, 1, 1, 1 } : ImVec4{ 1, 1, 1, 0.65 });
    ImGui::PushStyleColor(ImGuiCol_Button, state ? ImVec4{ 0.149, 0.509, 0.415, 1.0f } : ImVec4{ 1, 1, 1, 0.2 });
    if (ImGui::Button(label.c_str()))
    {
        uiFonction();
    }
    ImGui::PopStyleColor(2);

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}



bool UINoiseLayer::draw(unsigned int selectedLayerId)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    BeginGroupPanel("");
    
    ImGui::Text("> ");
    if (ImGui::IsItemClicked())
    {
        _open = _open ? false : true;
    }
    ImGui::SameLine();

    // Input Text for layer name
    // Prevent to use shortcuts for input text
    {
        ImGui::InputText(("##" + std::to_string(_id)).c_str(), (char*)_name.c_str(), _name.capacity() + 1);
        
        if (ImGui::IsItemFocused())
        {
            Editor::Get().setInputTextFocused(std::to_string(_id));
        }
        else if (Editor::Get().isInputTextFocused(std::to_string(_id)))
        {
            Editor::Get().setInputTextFocused("");
        }
    }
    
    if (_open)
    {
        ImGui::SameLine();

        drawIconButton(getIconLabel(ICON_MDI_EYE), "Enabled", _noiseLayer->isEnabled(), [this]()
        {
            _noiseLayer->toggleIsEnabled();
            Application::Get().Update(Ceres::ObserverFlag::MESH);
        });

        ImGui::SameLine();

        drawIconButton(getIconLabel(ICON_MDI_INVERT_COLORS), "Invert", _noiseLayer->isInverted(), [this]()
        {
            _noiseLayer->toggleIsInverted();
            Application::Get().Update(Ceres::ObserverFlag::MESH);
        });

        ImGui::SameLine();

        drawIconButton(getIconLabel(ICON_MDI_CIRCLE), "Isolate", _noiseLayer->isIsolated(), [this]()
        {
            _noiseLayer->toggleIsIsolated();
            _planet->getShapeGenerator()->setIsolatedLayerIndex(_noiseLayer->isIsolated() ? _id : -1);
            Application::Get().Update(Ceres::ObserverFlag::MESH);
        });


        drawParameter("Layer Type", [this]()
        {
            if (ImGui::Combo(("##Layer Type" + std::to_string(_id)).c_str(), &(int&)_noiseLayer->getNoiseSettings()->layerType, "Simple\0Rigid\0\0"))
            {
                _noiseLayer = _planet->getShapeGenerator()->updateLayerType(_id);
                _noiseSettingsParameters.setLayerType(_noiseLayer->getNoiseSettings()->layerType);
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        }, "Type of noise layer:\n * Simple: basic perlin noise\n * Rigid: sharp perlin noise (ideal for mountains).");

        drawParameter("Seed", [this]()
        {
            if (ImGui::InputInt(("##Seed" + std::to_string(_id)).c_str(), &(int&)_planet->getShapeGenerator()->getNoiseLayer(_id)->getSeed()))
            {
                _planet->getShapeGenerator()->getNoiseLayer(_id)->reseed();
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        }, "Random number to initialize perlin noise.");

        // Display layer noise settings
        _noiseSettingsParameters.display(_id, _sliderSpeed);
    }

    bool clicked = EndGroupPanel(selectedLayerId == _id);

    ImGui::PopStyleVar();

    return clicked;
}

std::string UINoiseLayer::getIconLabel(const char* icon)
{
    return std::string(icon) + "##" + std::to_string(_id);
}


}