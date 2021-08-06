#include "UiObserver.hpp"

#include "Interface.hpp"

namespace PlanetLab
{

UiObserver::UiObserver(Ceres::PlanetSubject& subject) 
    : _subject(subject)
{
    _subject.attach(this);
}

UiObserver::~UiObserver() {}


void UiObserver::updateResolution(int newResolution)
{
    Interface::Get().onResolutionUpdate(newResolution);
}

}