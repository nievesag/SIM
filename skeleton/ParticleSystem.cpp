#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Scene* scn)
	: scene(scn)
{
}

ParticleSystem::~ParticleSystem()
{
	for (auto g : particleGenerators) delete g;
	for (auto fg : forceGenerators) delete fg;
}

void ParticleSystem::step(double t)
{
	for (auto g : particleGenerators)
	{
		for (auto p : g->getParticles())
		{
			if (!p->getAlive()) 
			{
				g->deleteParticle(p);
			}
		}

		if (g != nullptr) g->step(t); // genera particula
		applyForces(g->getParticles()); // aplica las fuerzas a todas las particulas en este instante
	}
}

void ParticleSystem::applyForces(std::vector<Particle*>& generatedParticles)
{
	for (auto p : generatedParticles)
	{
		for (auto fg : forceGenerators)
		{
			if (p != nullptr)
			{
				p->addForce(fg->generateForce(*p)); // añade la fuerza generada 
			}
		}
	}
}