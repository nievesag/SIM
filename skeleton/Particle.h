#pragma once

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Entity.h"

using namespace physx;

class Particle : public Entity
{
public:
	Particle(Scene* scn, Vector3 pos, Vector3 vel, double siz, Vector4 col, float m, float damp, float maxLT);
	//Particle(Scene* scn, Vector3 pos, Vector3 vel, double siz, Vector4 col, float m, float damp, float maxLT, physx::PxShape* s);
	Particle(Scene* scn, Vector3 pos, Vector3 vel, double size);
	Particle(const Particle& model); // constructora con modelo
	Particle(const Particle& model, float newSize); // constructora con modelo para diferentes tamaños
	~Particle() override;

	// integradores: euler explicito y semi implicito
	virtual void step(double t) override;

	// setters
	void setAcc(Vector3 a) { acc = a; }					// aceleracion
	void setDamping(double d) { damping = d; }			// damping
	void setGravity(Vector3 g) { gravity = g; }			// gravedad
	void setLifetime(float l) { lifetime = l; }			// tiempo de vida
	void setMaxLifetime(float l) { maxLifetime = l; }	// tiempo de vida max

	// getters
	Vector3 getGravity() const { return gravity; }
	float getDamping() const { return damping; }
	float getLifetime() const { return lifetime; }			// tiempo de vida
	float getMaxLifetime() const { return maxLifetime; }	// tiempo de vida max
	bool getVisible() const { return visible; }

	// fuerzas
	virtual void addForce(float x, float y, float z) { resultingForce.push_back({ x,y,z }); }
	virtual void addForce(const Vector3& force)
	{
		resultingForce.push_back(force);
	}
	virtual void applyForce();

protected:
	std::vector<Vector3> resultingForce; // fuerza resultante

	// --- atributos
	// - fisicos
	Vector3 gravity = {0, -9.8f, 0}; // gravedad
	Vector3 acc = {0,0,0}; // aceleracion
	float damping = 0.99f; // damping -> v=v*d^t
	// coeficiente de damping:
	// si es 1 no hay rozamiento \ si es 0 no se mueve \ menos de 1->rozamiento ; mas de 1->deslizamiento

	// - gestion
	float lifetime;
	float maxLifetime = -1;

	virtual void integrate(double t);

	void manageLife(double t);
};