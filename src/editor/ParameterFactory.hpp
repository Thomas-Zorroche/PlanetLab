#pragma once

#include "Parameter.hpp"

class ParameterFactory
{
public:
	static std::pair <std::string, std::shared_ptr<ParameterBase> >
		Float(const std::string& name, FilterType type, ObserverFlag flag, float& value, float min, float max)
	{
		return { name, std::make_shared<ParameterFloat>(name, type, flag, value, min, max) };
	}

	static std::pair <std::string, std::shared_ptr<ParameterBase> >
		Int(const std::string& name, FilterType type, ObserverFlag flag, int& value, int min, int max)
	{
		return { name, std::make_shared<ParameterInt>(name, type, flag, value, min, max) };
	}

	static std::pair <std::string, std::shared_ptr<ParameterBase> >
		Boolean(const std::string& name, FilterType type, ObserverFlag flag, bool& value)
	{
		return { name, std::make_shared<ParameterBoolean>(name, type, flag, value) };
	}

	static std::pair <std::string, std::shared_ptr<ParameterBase> >
		Vec3(const std::string& name, FilterType type, ObserverFlag flag, glm::vec3& value, float min, float max)
	{
		return { name, std::make_shared<ParameterVec3>(name, type, flag, value, min, max) };
	}

	//static std::pair <std::string, std::shared_ptr<ParameterBase> >
	//	Color3(const std::string& name, FilterType type, ObserverFlag flag, const Color& color)
	//{
	//	return { name, std::make_shared<ParameterColor>(name, type, flag, color) };
	//}
};