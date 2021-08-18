#pragma once


namespace Ceres
{

/// Type of noise filters. Determine which algorithm will be used to evaluate noise
enum class FilterType
{
	Simple =0,
	Rigid
};

/**
 * @brief Contains all noise settings used by the noise filter to evaluate noise value 
 */
struct NoiseSettings
{
	enum FilterType filterType = FilterType::Simple;

	// ============================================
	// Simple filter type settings
	// ============================================

	float strength = 1.0f;
	
	float baseRoughness = 1.0f;
	
	float roughness = 2.0f;

	glm::vec3 center = glm::vec3(0, 0, 0);

	int iterations = 1;

	/// Amplitude will be half with each layer
	float persistence = 0.5f; 

	float minValue = 0.0f;


	// ============================================
	// Rigid filter type settings
	// ============================================

	float weightMultiplier = 0.5;
};

}	// ns Ceres

