#include "UINoiseLayer.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_groupPanel.hpp"

#include "editor/Application.hpp"
#include "editor/Parameter.hpp"

#include "Ceres/noise/NoiseLayer.hpp"


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
    ImGui::InputText(("##" + std::to_string(_id)).c_str(), (char*)_name.c_str(), _name.capacity() + 1);


    if (_open)
    {
        drawParameter("", [this]()
        {
            if (ImGui::Checkbox(("Enabled##" + std::to_string(_id)).c_str(), &_noiseLayer->isEnabled()))
            {
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        });

        drawParameter("", [this]()
        {
            if (ImGui::Checkbox(("Invert##" + std::to_string(_id)).c_str(), &_noiseLayer->isInverted()))
            {
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        });

        drawParameter("", [this]()
        {
            if (ImGui::Checkbox(("Isolate##" + std::to_string(_id)).c_str(), &_noiseLayer->isIsolated()))
            {
                _planet->getShapeGenerator()->setIsolatedLayerIndex(_noiseLayer->isIsolated() ? _id : -1);
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
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


}