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
	for (auto s : pSystems)
	{
		s->step(t);
	}

	for (auto e : gObjects)
	{
		e->step(t);
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

	// generadores
	ParticleSystem* sys = new ParticleSystem(this);
	ParticleGenerator* pg = new WaterfallGenerator(this, "Base");
	ForceGenerator* fg = new GravityGenerator({ 0,0,0 }, this, {0, -9.8, 0});
	sys->registerGenerator(pg);
	sys->registerForceGenerator(fg);

	pSystems.push_back(sys);
}

void Scene0::step(double t)
{
	Scene::step(t);
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
