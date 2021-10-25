#pragma once

#include "Ceres/noise/NoiseSettings.hpp"

#include "editor/Application.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace PlanetLab
{

void displayHoverDescription(const std::string& desc);

template <typename UIFonction>
void drawParameter(const std::string& name, UIFonction uiFonction, const std::string& desc = "")
{
	if (!name.empty())
	{
		float posX = (ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() * 0.4) - ImGui::CalcTextSize(name.c_str()).x
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		if (posX > ImGui::GetCursorPosX())
			ImGui::SetCursorPosX(posX);

		ImGui::AlignTextToFramePadding();
		ImGui::Text(name.c_str());
		ImGui::SameLine();
		displayHoverDescription(desc);
	}

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.4);

	uiFonction();

	if (name.empty() && !desc.empty())
	{
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.5, 0.5, 0.5, 1.0 });
		ImGui::Text("(?)");
		ImGui::PopStyleColor();
		displayHoverDescription(desc);
	}

	ImGui::PopItemWidth();
}


/*
* Abstract Parameter Class
*/
class ParameterBase
{
public:
	ParameterBase(const std::string& name, Ceres::LayerType type, Ceres::ObserverFlag flag, const std::string& desc = "")
		: _name(name), _type(type), _flag(flag), _description(desc)	{};

	virtual ~ParameterBase() {}

	Ceres::LayerType GetType() const { return _type; };

	virtual void Display(unsigned int layerId, float sliderSpeed = 0.1f) = 0;

protected:
	std::string _name = "";
	std::string _description = "";
	Ceres::LayerType _type = Ceres::LayerType::Simple;
	Ceres::ObserverFlag _flag = Ceres::ObserverFlag::MESH;
};

/*
* Concrete Parameter Classes 
*/
class ParameterFloat : public ParameterBase
{
public:
	ParameterFloat(const std::string& name, Ceres::LayerType type, Ceres::ObserverFlag flag, float& value, float min, float max, 
		const std::string& desc = "")
		: ParameterBase(name, type, flag, desc), _value(value), _min(min), _max(max) {};

	void Display(unsigned int layerId, float sliderSpeed = 0.1f) override
	{
		drawParameter(_name.c_str(), [this, layerId, sliderSpeed]()
		{
			if (ImGui::DragFloat(("##" + _name + std::to_string(layerId)).c_str(), &_value, sliderSpeed, _min, _max))
			{
				Application::Get().Update(_flag);
			}
		}, _description);
	};

private:
	float& _value;
	float _min;
	float _max;
};

class ParameterInt : public ParameterBase
{
public:
	ParameterInt(const std::string& name, Ceres::LayerType type, Ceres::ObserverFlag flag, int& value, int min, int max, 
		const std::string& desc = "")
		: ParameterBase(name, type, flag, desc), _value(value), _min(min), _max(max) {};


	void Display(unsigned int layerId, float sliderSpeed = 0.1f) override
	{
		drawParameter(_name.c_str(), [this, layerId]()
		{
			if (ImGui::SliderInt(("##" + _name + std::to_string(layerId)).c_str(), &_value, _min, _max))
			{
				Application::Get().Update(_flag);
			}
		}, _description);

	};

private:
	int& _value;
	int _min;
	int _max;
};

class ParameterBoolean : public ParameterBase
{
public:
	ParameterBoolean(const std::string& name, Ceres::LayerType type, Ceres::ObserverFlag flag, bool value, 
		const std::string& desc = "")
		: ParameterBase(name, type, flag, desc), _value(value) {};

	void Display(unsigned int layerId, float sliderSpeed = 0.1f) override
	{
		drawParameter(_name.c_str(), [this, layerId]()
		{
			if (ImGui::Checkbox((_name + std::to_string(layerId)).c_str(), &_value))
			{
				Application::Get().Update(_flag);
			}
		}, _description);
	};

private:
	bool& _value;
};


class ParameterVec3 : public ParameterBase
{
public:
	ParameterVec3(const std::string& name, Ceres::LayerType type, Ceres::ObserverFlag flag, glm::vec3& value, float min, float max,
		const std::string& desc = "")
		: ParameterBase(name, type, flag, desc), _value(value), _min(min), _max(max) {};

	void Display(unsigned int layerId, float sliderSpeed = 0.1f) override
	{
		drawParameter(_name, [this, layerId]()
		{
			if (ImGui::SliderFloat3(("##" + _name + std::to_string(layerId)).c_str(), (float*)&(_value), _min, _max))
			{
				Application::Get().Update(_flag);
			}
		}, _description);
	};

private:
	glm::vec3& _value;
	float _min;
	float _max;
};

}


