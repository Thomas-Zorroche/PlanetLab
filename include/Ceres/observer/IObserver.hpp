#pragma once

namespace Ceres
{

class IObserver 
{
public:
	virtual ~IObserver() {};

	virtual void updateResolution(int newResolution) = 0;
};

}