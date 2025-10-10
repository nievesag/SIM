#pragma once

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Particle.h"

class Scene;
using namespace physx;

class ProjectileGenerator
{
public:
	ProjectileGenerator(Vector3 Pos, Vector3 Vel, double size, char type, Scene* s);
	~ProjectileGenerator();

	// para definir la masa simulada Ms = Mr * (Vr^2 / Vs^2)
	// para definir la gravedad simulada Gs = Gr * (Vs^2 / Vr^2)

	// GetCamera()->getDir(); // direccion camera (para adaptar la velocidad de las particulas) => normalizar y multiplicar por el vector velocidad
	// GetCamera()->getEye(); // centro de la camara (posicion para inicializar las particulas)

	void shoot(char type);

private: 
	std::vector<Particle*> balas;
	Scene* scn = nullptr;
	std::pair<float, float> getSimulationAtributes(float massR, float gravityR, float vR, float vS);
};

