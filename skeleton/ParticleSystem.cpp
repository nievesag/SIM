#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
	for (auto g : particleGenerators) delete g;
}

void ParticleSystem::step(double t)
{
	for (auto g : particleGenerators) 
	{
		if (g != nullptr) g->step(t);
	}

	// afectar a las particulas
	for (auto e : vector particulas...) // recorrer particulas 
	{
		for (auto fg : vector fuerzas...) // recorrer fuerzas que afectan a esa particula 
		{
			// aplicar fuerza a esa particula
			fg->generateForce(*e);
		}
	}
}