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
	bool inArea(Entity& e);

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
	Vector3 gravity = {0, -2, 0}; // aceleracion de la gravedad
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

protected:
	Vector3 wind = { 0, 0, 10 }; // velocidad del viento

private:
	float k1 = 10;
	Vector3 k2 = { 0,0,0 };
};

class WhirlGenerator : public WindGenerator
{
public:
	WhirlGenerator(Vector3 pos, float areaR, Scene* scn, Vector3 _wind)
		: WindGenerator(pos, areaR, scn, _wind)
	{
		areaRadius = 200;
	}

	Vector3 generateForce(Entity& e) override;

private:
	float k = 1;
};

class MagnetismGenerator : public ForceGenerator 
{
public:
	MagnetismGenerator(Vector3 pos, float areaR, Scene* scn, float B);

	Vector3 generateForce(Entity& e) override;

private:
	float b = 0; // carga del campo magnetico
};