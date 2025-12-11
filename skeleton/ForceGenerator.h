#pragma once

#include <PxPhysicsAPI.h>

#include "core.hpp"
#include "RenderUtils.hpp"

#include "Particle.h"
class Magnet;
using namespace physx;

class ForceGenerator
{
public:
	ForceGenerator(Vector3 pos, float areaR, Scene* s, bool visibleArea);
	ForceGenerator() {}
	virtual ~ForceGenerator();

	// en el update de cada particula llamas a generateForce
	virtual Vector3 generateForce(Entity& e) = 0;

	PxTransform* getAreaPose() const { return areaPose; }

	void toggleForce() { isActive = !isActive; }

	void setAreaVisibility(bool v) { areaVisible = v; }

	void toggleAreaVisibility()
	{
		if (areaVisible) RegisterRenderItem(area);
		else DeregisterRenderItem(area);
	}

	void setActive(bool a) { isActive = a; }

protected:
	bool showArea();
	bool inArea(Entity& e);

	float areaRadius; // radio de area de actuacion
	RenderItem* area = nullptr;
	Vector3 areaPos = { 0,0,0 };
	PxTransform* areaPose = nullptr;

	bool areaVisible = true;

	bool isActive = true;
};

class GravityGenerator : public ForceGenerator
{
public:
	GravityGenerator(Vector3 pos, float areaR, Scene* scn, Vector3 _gravity)
		: ForceGenerator(pos, areaR, scn, false), gravity(_gravity)
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
		: ForceGenerator(pos, areaR, scn, true), wind(_wind)
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

#include "Magnet.h"

class MagnetismGenerator : public ForceGenerator 
{
public:
	MagnetismGenerator(Vector3 pos, float areaR, Scene* scn, float B, PxPhysics* gPhysics, PxScene* pxScn);

	Vector3 generateForce(Entity& e) override;

	void move(Vector3 dir);
	void setPos(Vector3 newPos);

	void toggleMagnetVisibility();

private:
	float b = 0; // carga del campo magnetico

	physx::PxTransform* magnetPose = nullptr;
	Magnet* magnet = nullptr;
};

class SpringForceGenerator : public ForceGenerator 
{
public:
	SpringForceGenerator(double _k, double _restingLength, Entity* _other);

	Vector3 generateForce(Entity& e) override;

	void setK(double _k) { k = _k; }

protected:
	double k; // coeficiente de elasticidad
	double restingLength; // longitud en reposo del muelle
	Entity* other; // particula con la que genera la fuerza
};

class BuoyancyForceGenerator : public ForceGenerator 
{
public:
	// h -> altura del liquido
	// v -> volumen del liquido
	// d -> densidad del liquido
	BuoyancyForceGenerator(Vector3 pos, float areaR, Vector3 liquidXZ, Scene* s, float h, float V, float d);

	Vector3 generateForce(Entity& e) override;

protected:
	float height;
	float volume;
	float liquidDensity;
	float gravity = 9.8;
	Vector3 liquidSize;
	RenderItem* fluid = nullptr;
};