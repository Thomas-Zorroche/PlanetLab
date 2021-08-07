﻿#pragma once

#include "ceres/noise/NoiseSettings.hpp"
#include "editor/Application.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


namespace PlanetLab
{

// TODO: remove duplicate function from Interface.cpp
template <typename UIFonction>
static void _drawParameter(const std::string& name, UIFonction uiFonction)
{
	if (!name.empty())
	{
		float posX = (ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() * 0.4) - ImGui::CalcTextSize(name.c_str()).x
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		if (posX > ImGui::GetCursorPosX())
			ImGui::SetCursorPosX(posX);

		ImGui::AlignTextToFramePadding();
		ImGui::Text(name.c_str()); ImGui::SameLine();
	}

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.4);

	uiFonction();

	ImGui::PopItemWidth();
}


/*
* Abstract Parameter Class
*/
class ParameterBase
{
public:
	ParameterBase(const std::string& name, Ceres::FilterType type, Ceres::ObserverFlag flag)
		: _name(name), _type(type), _flag(flag)	{};

	virtual ~ParameterBase() {}

	Ceres::FilterType GetType() const { return _type; };

	virtual void Display() = 0;

protected:
	std::string _name;
	Ceres::FilterType _type;
	Ceres::ObserverFlag _flag;
};

/*
* Concrete Parameter Classes 
*/
class ParameterFloat : public ParameterBase
{
public:
	ParameterFloat(const std::string& name, Ceres::FilterType type, Ceres::ObserverFlag flag, float& value, float min, float max)
		: ParameterBase(name, type, flag), _value(value), _min(min), _max(max) {};

	void Display() override 
	{
		_drawParameter(_name.c_str(), [&name = _name, &value = _value, &min = _min, &max = _max, &flag = _flag]()
		{
			if (ImGui::SliderFloat(("##" + name).c_str(), &value, min, max))
			{
				Application::Get().Update(flag);
			}
		});
	};

private:
	float& _value;
	float _min;
	float _max;
};

class ParameterInt : public ParameterBase
{
public:
	ParameterInt(const std::string& name, Ceres::FilterType type, Ceres::ObserverFlag flag, int& value, int min, int max)
		: ParameterBase(name, type, flag), _value(value), _min(min), _max(max) {};


	void Display() override
	{
		_drawParameter(_name.c_str(), [&name = _name, &value = _value, &min = _min, &max = _max, &flag = _flag]()
		{
			if (ImGui::SliderInt(("##" + name).c_str(), &value, min, max))
			{
				Application::Get().Update(flag);
			}
		});

	};

private:
	int& _value;
	int _min;
	int _max;
};

class ParameterBoolean : public ParameterBase
{
public:
	ParameterBoolean(const std::string& name, Ceres::FilterType type, Ceres::ObserverFlag flag, bool value)
		: ParameterBase(name, type, flag), _value(value) {};

	void Display() override
	{
		_drawParameter(_name.c_str(), [&name = _name, &value = _value, &flag = _flag]()
		{
			if (ImGui::Checkbox(name.c_str(), &value))
			{
				Application::Get().Update(flag);
			}
		});
	};

private:
	bool& _value;
};


class ParameterVec3 : public ParameterBase
{
public:
	ParameterVec3(const std::string& name, Ceres::FilterType type, Ceres::ObserverFlag flag, glm::vec3& value, float min, float max)
		: ParameterBase(name, type, flag), _value(value), _min(min), _max(max) {};

	void Display() override
	{
		_drawParameter(_name.c_str(), [&name = _name, &value = _value, &min = _min, &max = _max, &flag = _flag]()
		{
			if (ImGui::SliderFloat3(("##" + name).c_str(), (float*)&(value), min, max))
			{
				Application::Get().Update(flag);
			}
		});
	};

private:
	glm::vec3& _value;
	float _min;
	float _max;
};

}


