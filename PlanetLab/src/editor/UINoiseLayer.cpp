#include "UINoiseLayer.hpp"

#include "imgui/imgui.h"
#include "editor/Application.hpp"
#include "editor/Parameter.hpp"

#include "Ceres/noise/NoiseFilter.hpp"

namespace PlanetLab
{

UINoiseLayer::UINoiseLayer(std::shared_ptr<Ceres::NoiseLayer> noiseLayer,
        std::shared_ptr<Ceres::Planet> planet, float& sliderSpeed)
    : _noiseLayer(noiseLayer), 
	_id(noiseLayer->getId()), 
    _planet(planet),
    _sliderSpeed(sliderSpeed),
    _noiseSettingsParameters(noiseLayer->getNoiseSettings())
{
    _name = "Noise Layer " + std::to_string((_id + 1));
}

void UINoiseLayer::draw()
{
    ImGui::Separator();

    std::string titleNode = "Noise Layer " + std::to_string(_id + 1);
    if (ImGui::TreeNode(titleNode.c_str()))
    {
        drawParameter("", [this]()
        {
            if (ImGui::Checkbox("Enabled", &_noiseLayer->isEnabled()))
            {
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        });

        drawParameter("FilterType", [this]()
        {
            if (ImGui::Combo("##Filter Type", &(int&)_noiseLayer->getNoiseSettings()->filterType, "Simple\0Rigid\0\0"))
            {
                _planet->getShapeGenerator()->updateFilterType(_id);
                _noiseSettingsParameters.setFilterType(_noiseLayer->getNoiseSettings()->filterType);
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        }, "Type of noise filter:\n * Simple: basic perlin noise\n * Rigid: sharp perlin noise (ideal for mountains).");

        drawParameter("Seed", [this]()
        {
            if (ImGui::InputInt("##Seed", &(int&)_planet->getShapeGenerator()->getNoiseFilter(_id)->getSeed()))
            {
                _planet->getShapeGenerator()->getNoiseFilter(_id)->reseed();
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        }, "Random number to initialize perlin noise.");

        // Display filter noise settings
        _noiseSettingsParameters.display(_sliderSpeed);

        ImGui::TreePop();

    }
}


}