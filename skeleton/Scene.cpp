#include "Scene.h"

Scene::~Scene()
{

}

void Scene::init()
{

}

void Scene::step(double t) // update
{

}

void Scene::load()
{
	// para hacerlos visibles
	/*
	for (auto e : gObjects)
		e->setVisibility(true);
	*/
}

void Scene::unload()
{
	// para eliminarlos de la escena
	/*
	for (auto e : gObjects)
		e->setVisibility(false);
	*/
}

void Scene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{

}
