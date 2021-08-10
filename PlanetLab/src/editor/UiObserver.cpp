#include "UiObserver.hpp"

#include "Editor.hpp"

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
    Editor::Get().onResolutionUpdate(newResolution);
}

}