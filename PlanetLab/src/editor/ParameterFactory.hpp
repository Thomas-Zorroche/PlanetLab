#pragma once

#include "Parameter.hpp"


namespace PlanetLab
{


class ParameterFactory
{
public:
	static std::pair <std::string, std::shared_ptr<ParameterBase> >
		Float(const std::string& name, Ceres::LayerType type, float& value, float min, float max, const std::string& desc = "", 
			Ceres::ObserverFlag flag = Ceres::ObserverFlag::MESH)
	{
		return { name, std::make_shared<ParameterFloat>(name, type, flag, value, min, max, desc) };
	}

	static std::pair <std::string, std::shared_ptr<ParameterBase> >
		Int(const std::string& name, Ceres::LayerType type, int& value, int min, int max, const std::string& desc = "",
			Ceres::ObserverFlag flag = Ceres::ObserverFlag::MESH)
	{
		return { name, std::make_shared<ParameterInt>(name, type, flag, value, min, max, desc) };
	}

	static std::pair <std::string, std::shared_ptr<ParameterBase> >
		Boolean(const std::string& name, Ceres::LayerType type, bool& value, const std::string& desc = "",
			Ceres::ObserverFlag flag = Ceres::ObserverFlag::MESH)
	{
		return { name, std::make_shared<ParameterBoolean>(name, type, flag, value, desc) };
	}

	static std::pair <std::string, std::shared_ptr<ParameterBase> >
		Vec3(const std::string& name, Ceres::LayerType type, glm::vec3& value, float min, float max, const std::string& desc = "",
			Ceres::ObserverFlag flag = Ceres::ObserverFlag::MESH)
	{
		return { name, std::make_shared<ParameterVec3>(name, type, flag, value, min, max, desc) };
	}

	//static std::pair <std::string, std::shared_ptr<ParameterBase> >
	//	Color3(const std::string& name, LayerType type, ObserverFlag flag, const Color& color)
	//{
	//	return { name, std::make_shared<ParameterColor>(name, type, flag, color) };
	//}
};

}
