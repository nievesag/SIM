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
	for (auto e : gObjects)
	{
		e->step(t);
	}

	// llamar al step de los generadores

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

	// proyectiles
	/*
	float vel = 100;
	pGenerator = new ProjectileGenerator(GetCamera()->getEye(), GetCamera()->getDir().getNormalized()*vel, 5, 'h', this);
	*/

	// generadores
	ParticleSystem* sys = new ParticleSystem();

	ParticleGenerator* pg = new WaterfallGenerator(this, "Base");
	sys->registerGenerator(pg);
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
		if (pGenerator != nullptr)
		{
			pGenerator->shoot('h');
		}
		break;
	}
	default:
		break;
	}
}
