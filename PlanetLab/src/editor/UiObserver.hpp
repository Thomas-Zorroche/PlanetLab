#pragma once


#include "Ceres/observer/IObserver.hpp"
#include "Ceres/observer/ISubject.hpp"
#include "Ceres/observer/PlanetSubject.hpp"


namespace PlanetLab
{

class UiObserver : public Ceres::IObserver {
public:

    UiObserver(Ceres::PlanetSubject& subject);

    virtual ~UiObserver();

    virtual void updateResolution(int newResolution) override;

    virtual void updateMesh() override;



private:
    Ceres::PlanetSubject& _subject;
};

}

