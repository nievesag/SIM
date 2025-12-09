#pragma once

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Particle.h"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
#include "System.h"

using namespace physx;

// para gestionar conjuntos de particulas y organizar la escena
// cada sistema de particulas va a ser gestionado por si mismo con sus atributos

class ParticleSystem : public System
{
public:
	ParticleSystem(Scene* scn);
	~ParticleSystem();

	// update
	void step(double t) override;

	// registra el generador de particulas
	void registerGenerator(ParticleGenerator* g) 
	{
		particleGenerators.push_back(g);
	}

	// registra el generador de fuerza
	void registerForceGenerator(ForceGenerator* fg) 
	{
		forceGenerators.push_back(fg);
	}

protected:
	// vector de generadores
	std::vector<ParticleGenerator*> particleGenerators;
	std::vector<ForceGenerator*> forceGenerators; // generadores de fuerzas que afectan a cada particula
	// llamas a todos los generadores con la informacion de esa particula, el resultado sera la suma de todas las fuerzas que la afecten
	// el update de la particula sera fuerzaParticula = fuerza que recibe en ese instante del update

	void applyForces(std::vector<Particle*>& generatedParticles);
};