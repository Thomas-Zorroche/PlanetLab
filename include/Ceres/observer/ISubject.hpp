#pragma once


namespace Ceres
{

class IObserver;

class ISubject 
{

public:
	virtual ~ISubject() {};
	virtual void attach(IObserver* observer) = 0;
	virtual void detach(IObserver* observer) = 0;
	virtual void notify() = 0;

};
}

