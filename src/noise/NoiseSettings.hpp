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
	friend const std::shared_ptr<ParameterBase>& GetParameterByName(const ParametersMap& parameters, const std::string& name);

private:
	enum FilterType _filterType = FilterType::Simple;

	ParametersMap _parameters;
	std::vector<std::string> _parametersNames; // liste triée

	// Simple
	float _strength = 1.0f;
	float _baseRoughness = 1.0f;
	float _roughness = 2.0f;
	glm::vec3 _center = glm::vec3(0, 0, 0);
	int _layersCount = 1;
	float _persistence = 0.5f; // Amplitude will be half with each layer
	float _minValue = 0.0f;

	// Rigid
	float _weightMultiplier = 0.5;
};

