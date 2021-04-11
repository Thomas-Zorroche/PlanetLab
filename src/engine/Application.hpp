#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "engine/ProceduralEditor.hpp"
#include "engine/Log.hpp"
#include "engine/Window.hpp"
#include "engine/Scene.hpp"
#include "engine/Color.hpp"

void mainloop(Window& windowObject);

class Planet;

enum class UpdateMode
{
	Auto = 0,
	OnRelease,
	OnGenerate
};

enum class ObserverFlag
{
	RESOLUTION = 0,
	COLOR,
	RADIUS,
	NOISE,
	LAYER
};


class Application
{
public:
	Application(int argc, char** argv);
	~Application() {};

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	static Application& Get() { return *s_instance; }
public:
	void ClearColor() const;

	// Updates Handler
	void GenerateUpdateQueue(bool onRelease = false);
	void Update(ObserverFlag flag);
	void AddUpdateIntoQueue(ObserverFlag flag);
	UpdateMode& GetUpdateMode() { return _updateMode; }
	UpdateMode GetUpdateMode() const { return _updateMode; }

	Window& GetWindow() { return *_window; }
	ProceduralEditor& GetEditor() { return *_editor; }
	std::shared_ptr<Planet> GetPlanet() { return _planet; }

	void AppendPlanet(const std::shared_ptr<Planet>& planet) { _planet = planet; }

	Color GetBackgroundColor() const { return _backgroundColor; }
	Color& GetBackgroundColor() { return _backgroundColor; }

private:
	static Application* s_instance;

private:
	std::unique_ptr<Window> _window = nullptr;
	std::unique_ptr<ProceduralEditor> _editor = nullptr;
	std::shared_ptr<Planet> _planet = nullptr;
	
	UpdateMode _updateMode = UpdateMode::Auto;
	std::vector<ObserverFlag> _updatesQueue;

	Color _backgroundColor = Color(0.570, 0.598, 0.727);

};

