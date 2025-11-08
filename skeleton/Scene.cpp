#include "Scene.h"

#include "ProjectileGenerator.h"
#include "RenderUtils.hpp"
#include "ParticleSystem.h"
#include "ParticleGenerator.h"

Scene::~Scene()
{

}

void Scene::init()
{

}

void Scene::step(double t) // update
{
	deleteEntities();

	for (auto e : gObjects)
	{
		e->step(t);
	}

	for (auto s : pSystems)
	{
		s->step(t);
	}
}

void Scene::deleteEntities()
{
	int i = 0;
	for (auto g : gObjects)
	{
		if (!g->getAlive())
		{
			delete g;
			gObjects.erase(gObjects.begin()+i);
		}
		i++;
	}
}

void Scene::load()
{
	
}

void Scene::unload()
{
	// para eliminarlos de la escena
	for (auto e : gObjects)
		delete e;
	gObjects.clear();
}

void Scene::addEntity(Entity* ent)
{
	gObjects.push_back(ent);
}

void Scene::addSystem(ParticleSystem* sys)
{
	pSystems.push_back(sys);
}

void Scene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{

}

// ------------------------ ESCENAS HIJAS ------------------------
// Escena 0
void Scene0::init()
{
	Scene::init();

	// ----- System
	ParticleSystem* sys = new ParticleSystem(this);

	// -- Particle generators
	//ParticleGenerator* waterfallGenerator = new WaterfallGenerator(this, "Cascada");
	//sys->registerGenerator(waterfallGenerator);

	//ParticleGenerator* mistGenerator = new MistGenerator(this, "Niebla");
	//sys->registerGenerator(mistGenerator);

	//ParticleGenerator* fireworkGenerator = new FireworkGenerator(this, "Fuegos");
	//sys->registerGenerator(fireworkGenerator);

	trailGenerator = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trailGenerator);

	chargedGenerator = new ChargedGenerator(this, "Carga");
	sys->registerGenerator(chargedGenerator);

	ChargedEntity* bola = new ChargedEntity(this, { -20, 0,0 }, 3, -0.1, trailGenerator);
	chargedGenerator->addChargedEnitity(bola);

	/*
	// -- Force generators
	//ForceGenerator* gg = new GravityGenerator({ 0,0,0 }, 50, this, {0, -9.8, 0});
	//sys->registerForceGenerator(gg);

	//ForceGenerator* fg = new WindGenerator({ 0,0,0 }, 50, this, { 0, -10, 20 });
	//sys->registerForceGenerator(fg);

	//ForceGenerator* fg = new WhirlGenerator({ 0,0,0 }, 50, this, { 0, -10, 20 });
	//sys->registerForceGenerator(fg);
	*/

	magnetism = new MagnetismGenerator({ -50,0,0 }, 50, this, -0.01);
	sys->registerForceGenerator(magnetism);

	magnetism = new MagnetismGenerator({ 30,0,0 }, 50, this, 0.2);
	sys->registerForceGenerator(magnetism);

	pSystems.push_back(sys);
}

void Scene0::step(double t)
{
	Scene::step(t);

	if (!fatherPart)
	{
		fatherPart = true;
	}
}

void Scene0::load()
{
	Scene::load();
}

void Scene0::unload()
{
	Scene::unload();
}

void Scene0::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case 'H':
	{
		// disparo
		break;
	}
	default:
		break;
	}
}
