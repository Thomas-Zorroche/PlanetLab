#pragma once


namespace Ceres
{

/// Type of noise layesr. Determine which algorithm will be used to evaluate noise
enum class LayerType
{
	Simple =0,
	Rigid
};

/**
 * @brief Contains all noise settings used by the noise layer to evaluate noise value 
 */
struct NoiseSettings
{
	enum LayerType layerType = LayerType::Simple;

	// ============================================
	// Simple noise type settings
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
	// Rigid noise type settings
	// ============================================

	float weightMultiplier = 0.5;
};

}	// ns Ceres

