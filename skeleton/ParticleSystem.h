#pragma once

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Particle.h"
#include "ParticleGenerator.h"

using namespace physx;

// para gestionar conjuntos de particulas y organizar la escena
// cada sistema de particulas va a ser gestionado por si mismo con sus atributos

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	// update
	void step(double t);

	// generar el generador
	void generateGenerator(ParticleGenerator* g);

private:
	// vector de generadores
	std::vector<ParticleGenerator*> particleGenerators;
};

