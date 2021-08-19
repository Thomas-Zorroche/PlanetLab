﻿#include "UINoiseLayer.hpp"

#include "imgui/imgui.h"
#include "editor/Application.hpp"
#include "editor/Parameter.hpp"

#include "Ceres/noise/NoiseLayer.hpp"

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

        drawParameter("", [this]()
        {
            if (ImGui::Checkbox("Invert", &_noiseLayer->isInverted()))
            {
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        });

        drawParameter("", [this]()
        {
            if (ImGui::Checkbox("Isolate", &_noiseLayer->isIsolated()))
            {
                _planet->getShapeGenerator()->setIsolatedLayerIndex(_noiseLayer->isIsolated() ? _id : -1);
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        });

        drawParameter("Layer Type", [this]()
        {
            if (ImGui::Combo("##Layer Type", &(int&)_noiseLayer->getNoiseSettings()->layerType, "Simple\0Rigid\0\0"))
            {
                _noiseLayer = _planet->getShapeGenerator()->updateLayerType(_id);
                _noiseSettingsParameters.setLayerType(_noiseLayer->getNoiseSettings()->layerType);
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        }, "Type of noise layer:\n * Simple: basic perlin noise\n * Rigid: sharp perlin noise (ideal for mountains).");

        drawParameter("Seed", [this]()
        {
            if (ImGui::InputInt("##Seed", &(int&)_planet->getShapeGenerator()->getNoiseLayer(_id)->getSeed()))
            {
                _planet->getShapeGenerator()->getNoiseLayer(_id)->reseed();
                Application::Get().Update(Ceres::ObserverFlag::MESH);
            }
        }, "Random number to initialize perlin noise.");

        // Display layer noise settings
        _noiseSettingsParameters.display(_sliderSpeed);

        ImGui::TreePop();

    }
}


}