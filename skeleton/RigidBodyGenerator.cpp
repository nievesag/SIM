#include "RigidBodyGenerator.h"

RigidBodyGenerator::RigidBodyGenerator(Scene* s, std::string mod, PxPhysics* gphys, PxScene* gscn)
	: scn(s), model(mod), gphys(gphys), gscn(gscn) 
{
	// --- crear modelos de particulas
	// - cascada
	RigidBodyDynamic* modeloWaterfall = new RigidBodyDynamic(scn, gphys, gscn);
	rbs.emplace(std::make_pair(std::string("Cascada"), modeloWaterfall));
	DeregisterRenderItem(modeloWaterfall->getRenderItem());
}

RigidBodyGenerator::~RigidBodyGenerator()
{
	rbs.clear();
}

void RigidBodyGenerator::step(double t)
{
	deleteEntities();
	generateRb(); // creacion de las particulas
}

void RigidBodyGenerator::addEntity(RigidBodyDynamic* p)
{
	generatedRb.push_back(p);
	scn->addEntity(p);
}

void RigidBodyGenerator::deleteEntities()
{
	for (auto p : generatedRb)
	{
		if (!p->getAlive())
		{
			auto it = std::find(generatedRb.begin(), generatedRb.end(), p);

			if (it != generatedRb.end())
			{
				generatedRb.erase(it);
			}
		}
	}
}
