#pragma once

#include "glm/glm.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>


enum class FilterType
{
	Simple =0,
	Rigid
};

class ParameterBase;
using ParametersMap = std::unordered_map < std::string, std::shared_ptr<ParameterBase> >;


class NoiseSettings
{
public:

public:
	NoiseSettings();

	void Display();

	float strength() const;
	float& strength();

	float baseRoughness() const;
	float& baseRoughness();

	float roughness() const;
	float& roughness();

	glm::vec3 center() const;
	glm::vec3& center();

	int layersCount() const;
	int& layersCount();

	float persistence() const;
	float& persistence();

	float minValue() const;
	float& minValue();

	float weightMultiplier() const;
	float& weightMultiplier();

	FilterType& GetFilterType();

private:
	friend float Float(const ParametersMap& parameters, const std::string& name);
	friend float& FloatPtr(ParametersMap& parameters, const std::string& name);

	friend int Int(const ParametersMap& parameters, const std::string& name);
	friend int& IntPtr(ParametersMap& parameters, const std::string& name);

	friend glm::vec3 Vec3(const ParametersMap& parameters, const std::string& name);
	friend glm::vec3& Vec3Ptr(ParametersMap& parameters, const std::string& name);

	friend const std::shared_ptr<ParameterBase>& GetParameterByName(const ParametersMap& parameters, const std::string& name);
	friend std::shared_ptr<ParameterBase>& GetParameterPtrByName(ParametersMap& parameters, const std::string& name);

private:
	enum FilterType _filterType = FilterType::Simple;

	ParametersMap _parameters;
	std::vector<std::string> _parametersNames; // liste triée
};

