#pragma once

#include "Ceres/Planet.hpp"
#include "editor/NoiseSettingsParameters.hpp"
#include "engine/SpriteSheet.hpp"
#include "engine/opengl/Framebuffer.hpp"

namespace PlanetLab
{

struct MeshStatistics
{
	std::string verticesCount = "";
	std::string facesCount = "";
	std::string trianglesCount = "";
};


class Viewer3DPanel
{
public:
	Viewer3DPanel(std::shared_ptr<Ceres::Planet> planet = nullptr);

	void draw();

	float getViewportWidth() const { return _viewportWidth; }
	float getViewportHeight() const { return _viewportHeight; }

	void setPlanet(std::shared_ptr<Ceres::Planet> planet) { _planet = planet; }

	void updateStatistics();

	void bindFbo() { _fbo.bind(_viewportWidth, _viewportHeight); }
	void unbindFbo() { _fbo.unbind(); }
	void freeFbo() { _fbo.free(); }

	bool isViewer3DHovered() const { return _viewer3DHovered; }

private:
	void displayRenderer();
	void displayStatistics();
	void displayLoadingWheel();


	MeshStatistics _statistics;


private:
	float _viewportWidth = 500.0f;
	float _viewportHeight = 300.0f;

	Framebuffer _fbo = Framebuffer();

	std::shared_ptr<Ceres::Planet> _planet;

	SpriteSheet _loadingWheel = SpriteSheet("res/img/LoadingSheet.png", 31);

	bool _viewer3DHovered = false;
};

void prettyPrintNumber(int number, std::string& str);

}