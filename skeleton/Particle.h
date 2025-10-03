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
	Particle(Vector3 Pos, Vector3 Vel, double size);
	~Particle() override;

	// integradores: euler explicito y semi implicito
	void step(double t) override;

	//void step(double t); // actualizar particula
	//void cleanUp(); // borrar
	//void init(); // inicializar particula

	void setPos(PxTransform* p) { pose = p; }	// pose
	void setAcc(Vector3 a) { acc = a; }			// aceleracion
	void setMass(double m) { mass = m; }		// masa
	void setDamping(double d) { damping = d; }	// damping
	void setVel(Vector3 v) { vel = v; }			// velocidad
	void setGravity(float g) { gravity = g; }	// gravedad
	void setShotAcc(float s) { shotAcc = s; }	// aceleracion de tiro

	float getGravity() { return gravity; }

private:
	Vector3 vel; // solo cambia con la aceleracion y la aceleracion cambia mediante una fuerza
	PxTransform* pose = nullptr; // A render item le pasaremos la direccion de este pose, para que se actualice automaticamente
	RenderItem* renderItem = nullptr; // para poder renderizarla
	float gravity = -9.8f;
	float shotAcc = 0;
	Vector3 acc = {shotAcc,gravity,0}; // aceleracion
	double mass = 0;

	// damping -> v=v*d^t
	// coeficiente de damping, si es 1 no hay rozamiento \ si es 0 no se mueve \ menos de 1->rozamiento ; mas de 1->deslizamiento
	double damping = 0.99;

	void integrate(double t);
};

