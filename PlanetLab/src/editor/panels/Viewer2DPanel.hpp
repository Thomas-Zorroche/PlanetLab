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

	bool isViewer2DHovered() const { return _viewer2DHovered; }

	void zoom(float zoom) 
	{ 
		_zoom += zoom; 
		if (_zoom < 0.1) _zoom = 0.1;
	}

private:
	std::shared_ptr<Ceres::Planet> _planet;
	
	unsigned int _textureId = 0;
	ImVec2 _textureSize = ImVec2{ 256, 256 };

	std::vector<unsigned char> localBuffer = std::vector<unsigned char>(_textureSize.x * _textureSize.y);

	UpdateMode _updateMode = UpdateMode::OnGenerate;

	bool _readyToGenerate = false;

	bool _viewer2DHovered = false;

	float _zoom = 1.0f;
};

}