#pragma once

#include "Particle.h"

class Scene;

class System
{
public:
	System(Scene* scn) : scene(scn) { }
	~System() { }

	//virtual void applyForces(std::vector<Entity*>& generatedParticles) { }

	virtual void step(double t) { }

protected:
	Scene* scene = nullptr;
};