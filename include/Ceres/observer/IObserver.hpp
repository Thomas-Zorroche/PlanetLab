#pragma once

namespace Ceres
{

/**
* @brief Interface for Observer object in Observer pattern
*/
class IObserver 
{
public:
	virtual ~IObserver() {};

	virtual void updateResolution(int newResolution) = 0;
};

}