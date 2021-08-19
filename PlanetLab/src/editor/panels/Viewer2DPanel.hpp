#pragma once

#include "Ceres/Planet.hpp"
#include "engine/opengl/Texture.h"
#include "imgui/imgui.h"
#include "editor/Application.hpp"


namespace PlanetLab
{


class Viewer2DPanel
{
public:
	Viewer2DPanel(std::shared_ptr<Ceres::Planet> planet = nullptr);
	~Viewer2DPanel();

	void draw();

	void generateTexture();

	void updateTexture();

	void setPlanet(std::shared_ptr<Ceres::Planet> planet) { _planet = planet; }

	void drawUpdateModeItem();


private:
	std::shared_ptr<Ceres::Planet> _planet;
	
	unsigned int _textureId = 0;
	ImVec2 _textureSize = ImVec2{ 256, 256 };

	std::vector<unsigned char> localBuffer = std::vector<unsigned char>(_textureSize.x * _textureSize.y);

	UpdateMode _updateMode = UpdateMode::OnGenerate;

	bool _readyToGenerate = false;

	int _noiseLayerId = -1;

};

}