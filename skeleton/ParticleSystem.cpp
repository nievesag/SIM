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
}

void ParticleSystem::generateGenerator(ParticleGenerator* g)
{
	particleGenerators.push_back(g);
}
