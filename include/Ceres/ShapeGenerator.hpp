#pragma once


namespace Ceres
{

class NoiseLayer;

/**
 * @brief Responsible to apply noises algorithms on the surface mesh
 */
class ShapeGenerator
{
public:
	ShapeGenerator();

	// Layers

	std::vector<std::shared_ptr<NoiseLayer> > getNoiseLayers();

	std::shared_ptr<NoiseLayer> getNoiseLayer(unsigned int index) const;

	std::shared_ptr<NoiseLayer> updateLayerType(std::uint32_t index);

	void addLayer(const std::shared_ptr<NoiseLayer>& layer);

	void removeLastLayer();

	void removeLayer(unsigned int index);

	void removeAllLayers();

	void setIsolatedLayerIndex(int index);


	// Elevation

	/**
	 * @brief Compute elevation of the given point after noise deformation
	 * 
	 * @param pointOnUnitSphere point on the unit normalize sphere
	 * @return elevation value (positive or negative)
	 */
	float calculateUnscaledElevation(const glm::vec3& pointOnUnitSphere) const;

	/**
	 * @brief Compute elevation from the surface of the water
	 *
	 * @param unscaledElevation elevation from noise
	 * @return elevation from water (only positive)
	 */
	float getScaledElevation(float unscaledElevation) const;

private:
	std::vector<std::shared_ptr<NoiseLayer> > _noiseLayers;

	float _planetRadius = 1.0f;

	int _isolatedLayerIndex = -1;
};

}	// ns proceduralPlanet