#pragma once

#include <PxPhysicsAPI.h>

#include <vector>
#include <unordered_map>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Particle.h"

class Scene;
using namespace physx;

class ForceGenerator
{
public:
	ForceGenerator(Vector3 pos, Scene* s);
	virtual ~ForceGenerator();

protected:
	bool showArea() 
	{
		// toggle area view
		// return !area
		return true;
	}

	float areaRadius; // radio de area de actuacion
	RenderItem* area = nullptr;
	// en el update de cada particula llamas a generateForce
	virtual Vector3 generateForce(Entity& e) = 0;
	Vector3 areaPos = { 0,0,0 };
};

class GravityGenerator : public ForceGenerator
{
public:
	GravityGenerator(Vector3 pos, Scene* scn, Vector3 _gravity)
		: ForceGenerator(pos, scn), gravity(_gravity)
	{

	}

	Vector3 generateForce(Entity& e) override;

private:
	Vector3 gravity = {0, -9.8, 0}; // aceleracion de la gravedad
};

class WindGenerator 
{
	
};