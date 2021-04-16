#pragma once

#include <iostream>
#include <string>
#include <variant>
#include "noise/NoiseSettings.hpp"
#include "engine/Application.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using ParameterType = std::variant<float, int, bool, glm::vec3>;

/*
* Abstract Parameter Class
*/
class ParameterBase
{
public:
	ParameterBase(const std::string& name, FilterType type, ObserverFlag flag)
		: _name(name), _type(type), _flag(flag)	{};

	virtual ~ParameterBase() {}

	FilterType GetType() const { return _type; };

	//ParameterType GetValue() const { return _value; }
	//ParameterType& GetValue() { return _value; }

	virtual void Display() = 0;

protected:
	std::string _name;
	FilterType _type;
	ObserverFlag _flag;
};

/*
* Concrete Parameter Classes 
*/
class ParameterFloat : public ParameterBase
{
public:
	ParameterFloat(const std::string& name, FilterType type, ObserverFlag flag, float& value, float min, float max)
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
	ParameterInt(const std::string& name, FilterType type, ObserverFlag flag, int& value, int min, int max)
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
	ParameterBoolean(const std::string& name, FilterType type, ObserverFlag flag, bool value)
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

//class ParameterColor : public ParameterBase
//{
//public:
//	ParameterColor(const std::string& name, FilterType type, ObserverFlag flag, const Color& defaultValue)
//		: ParameterBase(name, type, flag), _value(defaultValue) {};
//
//	void Display() override
//	{
//		if (ImGui::ColorEdit3(_name.c_str(), (float*)&(_value)))
//		{
//			Application::Get().Update(_flag);
//		}
//	};
//
//private:
//	Color _value;
//};

class ParameterVec3 : public ParameterBase
{
public:
	ParameterVec3(const std::string& name, FilterType type, ObserverFlag flag, glm::vec3& value, float min, float max)
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

