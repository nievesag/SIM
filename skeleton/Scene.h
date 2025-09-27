#pragma once
#include <vector>
#include <foundation/PxTransform.h>

#include "Entity.h"

class Scene
{
public:
	Scene();
	~Scene();

	virtual void init();

	void step(double t); // update

	void load();
	void unload();

	virtual void keyPressed(unsigned char key, const physx::PxTransform& camera) {}

protected:
	std::vector<Entity*> gObjects;		// Entidades de la escena
};

