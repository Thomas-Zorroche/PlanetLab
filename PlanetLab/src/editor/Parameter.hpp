#pragma once

#include "ceres/noise/NoiseSettings.hpp"
#include "editor/Application.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


namespace PlanetLab
{

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
		if (ImGui::SliderFloat(_name.c_str(), &_value, _min, _max))
		{
			Application::Get().Update(_flag);
		}
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
		if (ImGui::SliderInt(_name.c_str(), &_value, _min, _max))
		{
			Application::Get().Update(_flag);
		}
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
		if (ImGui::Checkbox(_name.c_str(), &_value))
		{
			Application::Get().Update(_flag);
		}
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
		if (ImGui::SliderFloat3(_name.c_str(), (float*)&(_value), _min, _max))
		{
			Application::Get().Update(_flag);
		}
	};

private:
	glm::vec3& _value;
	float _min;
	float _max;
};

}


