#pragma once

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

using namespace physx;

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel, double size);
	~Particle();

	// integradores: euler explicito y semi implicito
	void integrate(double t);

	//void step(double t); // actualizar particula
	//void cleanUp(); // borrar
	//void init(); // inicializar particula

private:
	Vector3 vel; // solo cambia con la aceleracion y la aceleracion cambia mediante una fuerza
	PxTransform* pose; // A render item le pasaremos la direccion de este pose, para que se actualice automaticamente
	RenderItem* renderItem = nullptr; // para poder renderizarla
	Vector3 acc = {1,0,0}; // aceleracion

	// damping -> v=v*d^t
	// coeficiente de damping, si es 1 no hay rozamiento \ si es 0 no se mueve \ menos de 1->rozamiento ; mas de 1->deslizamiento
	double damping = 0.5; 
};

