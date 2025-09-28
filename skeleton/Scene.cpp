#include "Scene.h"

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

void Scene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{

}

// ------------------------ ESCENAS HIJAS ------------------------
// Escena 0
void Scene0::init()
{
	Scene::init();
	Vector3 vel = { 0,0,0 };
	Particle* p = new Particle({ 0,0,0 }, vel, 5);
	addEntity(p);
}

void Scene0::step(double t)
{
	Scene::step(t);
}

void Scene0::load()
{
	Scene::load();
	Vector3 vel = { 0,0,0 };
	Particle* p = new Particle({ 0,0,0 }, vel, 5);
	addEntity(p);
}

void Scene0::unload()
{
	Scene::unload();
}

void Scene0::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
}
