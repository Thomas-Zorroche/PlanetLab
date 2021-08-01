#include "PlanetSubject.hpp"

namespace Ceres
{

void PlanetSubject::attach(IObserver* observer)
{
    _observers.push_back(observer);
}

void PlanetSubject::detach(IObserver* observer) 
{
    _observers.remove(observer);
}

void PlanetSubject::notify() 
{

}

void PlanetSubject::updateResolution(int newResolution)
{
    std::list<IObserver*>::iterator iterator = _observers.begin();
    while (iterator != _observers.end()) 
    {
        (*iterator)->updateResolution(newResolution);
        ++iterator;
    }
}



}