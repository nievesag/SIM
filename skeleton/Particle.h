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
	Particle(Scene* scn, Vector3 Pos, Vector3 Vel, double size, Vector4 color, float m, double damp);
	Particle(Scene* scn, Vector3 Pos, Vector3 Vel, double size);
	~Particle() override;

	// integradores: euler explicito y semi implicito
	void step(double t) override;

	// getters
	void setAcc(Vector3 a) { acc = a; }			// aceleracion
	void setDamping(double d) { damping = d; }	// damping
	void setGravity(Vector3 g) { gravity = g; }	// gravedad
	void setShotAcc(float s) { shotAcc = s; }	// aceleracion de tiro

	// setters
	Vector3 getGravity() { return gravity; }
	float getDamping() { return damping; }

	// fuerzas
	void addForce(float x, float y, float z) { resultingForce.push_back({ x,y,z }); }
	void addForce(Vector3 force) { resultingForce.push_back(force); }
	void applyForce();

private:
	std::vector<Vector3> resultingForce; // fuerza resultante

	// --- atributos
	Vector3 gravity = {0, -9.8f, 0}; // gravedad
	float shotAcc = 0;
	Vector3 acc = {0,0,0}; // aceleracion

	// damping -> v=v*d^t
	// coeficiente de damping, si es 1 no hay rozamiento \ si es 0 no se mueve \ menos de 1->rozamiento ; mas de 1->deslizamiento
	double damping = 0.99;

	void integrate(double t);
};

