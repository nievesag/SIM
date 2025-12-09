#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem(Scene* scn, PxPhysics* gphys, PxScene* gscn)
	: System(scn), gphys(gphys), gscn(gscn)
{
}

RigidBodySystem::~RigidBodySystem()
{
	for (auto g : rbGenerators) delete g;
	for (auto fg : forceGenerators) delete fg;
}

void RigidBodySystem::step(double t)
{
	for (auto g : rbGenerators)
	{
		for (auto p : g->getRbs())
		{
			if (!p->getAlive())
			{
				g->deleteEntities();
			}
		}

		if (g != nullptr) g->step(t); // genera rb
		applyForces(g->getRbs()); // aplica las fuerzas a todas las particulas en este instante
	}
}

void RigidBodySystem::applyForces(std::vector<RigidBodyDynamic*>& generatedRb)
{
	for (auto p : generatedRb)
	{
		for (auto fg : forceGenerators)
		{
			if (p != nullptr && fg != nullptr)
			{
				p->addForce(fg->generateForce(*p)); // añade la fuerza generada 
			}
		}
	}
}
