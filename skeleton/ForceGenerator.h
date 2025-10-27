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
	ForceGenerator(Vector3 pos, float areaR, Scene* s);
	virtual ~ForceGenerator();

	// en el update de cada particula llamas a generateForce
	virtual Vector3 generateForce(Entity& e) = 0;

protected:
	bool showArea();

	float areaRadius; // radio de area de actuacion
	RenderItem* area = nullptr;
	Vector3 areaPos = { 0,0,0 };
};

class GravityGenerator : public ForceGenerator
{
public:
	GravityGenerator(Vector3 pos, float areaR, Scene* scn, Vector3 _gravity)
		: ForceGenerator(pos, areaR, scn), gravity(_gravity)
	{
		areaRadius = 200;
	}

	Vector3 generateForce(Entity& e) override;

private:
	Vector3 gravity = {0, -9.8, 0}; // aceleracion de la gravedad
};

class WindGenerator : public ForceGenerator
{
public:
	WindGenerator(Vector3 pos, float areaR, Scene* scn, Vector3 _wind)
		: ForceGenerator(pos, areaR, scn), wind(_wind)
	{
		areaRadius = 200;
	}

	Vector3 generateForce(Entity& e) override;

private:
	Vector3 wind = { 0, 0, 10 }; // velocidad del viento
};

class WhirlGenerator : public ForceGenerator
{
public:
	WhirlGenerator(Vector3 pos, float areaR, Scene* scn, Vector3 _wind)
		: ForceGenerator(pos, areaR, scn), wind(_wind)
	{
		areaRadius = 200;
	}

	Vector3 generateForce(Entity& e) override;

private:
	Vector3 wind = { 0, 0, 10 }; // velocidad del viento
};