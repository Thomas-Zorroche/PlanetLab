#pragma once
#include "engine/Application.hpp"
#include "glm/glm.hpp"
#include "engine/Color.hpp"

#include <memory>

class Camera;
class Window;
class Camera;

class Hud
{
public:
	static Hud& get()
	{
		static Hud instance;
		return instance;
	}

	Hud(const Hud&) = delete;
	Hud& operator=(const Hud&) = delete;

	void draw(const std::shared_ptr<Camera>& camera) const;
	void init(GLFWwindow* window, float width, float height);
	void free();
	void bindFbo();
	void Hud::unbindFbo();

	float viewportWidth() const { return _viewportWidth; }
	float viewportHeight() const { return _viewportHeight; }

	/* UI Fonctions */
	bool wireframeMode() const { return _wireframeMode; }
	int resolution() const { return _resolution; }
	const Color& planetColor() const { return _planetColor; }
	float planetRadius() const { return _planetRadius; }

private:
	Hud() = default;
	~Hud();

	GLuint _fboViewport = -1;

	static bool _wireframeMode;
	static int _resolution;
	static Color _planetColor;
	static float _planetRadius;

	float _viewportWidth;
	float _viewportHeight;
	float _settingsWidth;
	
};