#pragma once

#include "ISubject.hpp"
#include "IObserver.hpp"


namespace Ceres
{

class PlanetSubject : public ISubject {
public:
    virtual ~PlanetSubject() {}

	void attach(IObserver* observer) override;

    void detach(IObserver* observer) override;

    void notify() override;

    void updateResolution(int newResolution);

    void updateMesh();

private:
    std::list<IObserver *> _observers;
};

}