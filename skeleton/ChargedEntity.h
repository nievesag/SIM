#pragma once
#include "Particle.h"
class TrailGenerator;

class ChargedEntity : public Particle
{
public:
	ChargedEntity(Scene* scn, Vector3 pos, float size, float q, TrailGenerator* _trailGenerator);
	~ChargedEntity(); 

	void step(double t) override;

private:
	TrailGenerator* trailGenerator = nullptr;
};

