#include "Scene.h"
#include "ProjectileGenerator.h"

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
	//Vector3 vel = { 0,0,0 };
	//Particle* p = new Particle({ 0,0,0 }, vel, 5);
	//addEntity(p);

	Vector3 vel = { -100,0,0 };
	pGenerator = new ProjectileGenerator({ 0,0,0 }, vel, 5, 'h', this);
}

void Scene0::step(double t)
{
	Scene::step(t);
}

void Scene0::load()
{
	Scene::load();
	//Vector3 vel = { -100,0,0 };
	//Particle* p = new Particle({ 0,0,0 }, vel, 5);
	//addEntity(p);

	Vector3 vel = { -100,0,0 };
	// FALTA PASARLE LOS VALORES PARA QUE SALGA DE LA CAMARA !!!!!!!
	pGenerator = new ProjectileGenerator({0,0,0}, vel , 5, 'h', this);
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
