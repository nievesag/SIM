#pragma once

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
#include "RigidBody.h"
#include "RigidBodyGenerator.h"

using namespace physx;

class RigidBodySystem : public System
{
public:
	RigidBodySystem(Scene* scn, PxPhysics* gphys, PxScene* gscn);
	
	~RigidBodySystem();

	// update
	void step(double t) override;

	// registra el generador de rb
	void registerGenerator(RigidBodyGenerator* g)
	{
		rbGenerators.push_back(g);
	}

	// registra el generador de fuerza
	void registerForceGenerator(ForceGenerator* fg)
	{
		forceGenerators.push_back(fg);
	}

protected:
	// vector de generadores
	std::vector<RigidBodyGenerator*> rbGenerators;
	std::vector<ForceGenerator*> forceGenerators; // generadores de fuerzas que afectan a cada particula
	// llamas a todos los generadores con la informacion de esa particula, el resultado sera la suma de todas las fuerzas que la afecten
	// el update de la particula sera fuerzaParticula = fuerza que recibe en ese instante del update

	void applyForces(std::vector<RigidBodyDynamic*>& generatedRb);

	PxPhysics* gphys;
	PxScene* gscn;
};