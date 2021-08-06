#pragma once


namespace Ceres
{

class IObserver;

/**
 * @brief Interface for Subject object in Observer pattern
 */
class ISubject 
{

public:
	virtual ~ISubject() {};

	virtual void attach(IObserver* observer) = 0;

	virtual void detach(IObserver* observer) = 0;

	virtual void notify() = 0;

};
}

