#pragma once

#include <PxPhysicsAPI.h>

#include "core.hpp"
#include "RenderUtils.hpp"

#include "Particle.h"
//#include "ParticleSystem.h"
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

	virtual void toggleForce() { isActive = !isActive; }

	void setAreaVisibility(bool v) { areaVisible = v; }

	virtual void toggleAreaVisibility()
	{
		if (areaVisible) RegisterRenderItem(area);
		else DeregisterRenderItem(area);
	}

	void showArea(bool value) {
		value ? RegisterRenderItem(area) : DeregisterRenderItem(area);
	}

	void showArea() {
		RegisterRenderItem(area);
	}

	void hideArea() {
		DeregisterRenderItem(area);
	}

	void setActive(bool a) { isActive = a; }

	void setAreaPos(Vector3 pos)
	{
		areaPos = pos;
		areaPose = new PxTransform(pos);
	}

protected:
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
		
	}

	Vector3 generateForce(Entity& e) override;

private:
	Vector3 gravity = {0, -2, 0}; // aceleracion de la gravedad
};

class WindGenerator : public ForceGenerator
{
public:
	WindGenerator(Vector3 pos, float areaR, Scene* scn, Vector3 _wind)
		: ForceGenerator(pos, areaR, scn, false), wind(_wind) { }

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
	WhirlGenerator(Vector3 pos, float areaR, Scene* scn, Vector3 _wind, float _k)
		: WindGenerator(pos, areaR, scn, _wind), k(_k)
	{
		
	}

	Vector3 generateForce(Entity& e) override;

private:
	float k = 1;
};

class HorizontalWhirlGenerator : public WindGenerator
{
public:
	HorizontalWhirlGenerator(Vector3 pos, float areaR, Scene* scn, Vector3 _wind)
		: WindGenerator(pos, areaR, scn, _wind)
	{
		
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
	void toggleForce() override;
	void toggleMagnetVisibility();
	void toggleAreaVisibility() override;
	Magnet* getMagnet() { return magnet; }
	float getB() { return b; }

private:
	float b = 0; // carga del campo magnetico

	physx::PxTransform* magnetPose = nullptr;
	Magnet* magnet = nullptr;
	PxScene* s = nullptr;
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
	void toggleAreaVisibility() override
	{
		ForceGenerator::toggleAreaVisibility();

		if (areaVisible) RegisterRenderItem(fluid);
		else DeregisterRenderItem(fluid);
	}
	void hideWater() { DeregisterRenderItem(fluid); }
	void showWater() { RegisterRenderItem(fluid); }

protected:
	float height;
	float volume;
	float liquidDensity;
	float gravity = 9.8;
	Vector3 liquidSize;
	RenderItem* fluid = nullptr;
};

class ExplosionForceGenerator : public ForceGenerator
{
public:
	ExplosionForceGenerator(Vector3 pos, float areaR, Scene* s, bool visibleArea, float _k, float _t);

	Vector3 generateForce(Entity& e) override;

private:
	float k; // intensidad de la explosion
	float t; // constante de tiempo de la explosion
	// a partir de 4t la explosion casi se ha desvanecido

	float duration = 100;
};