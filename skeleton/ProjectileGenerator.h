
#pragma once

#pragma once

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include <unordered_map>

#include "Particle.h"
#include "Projectile.h"

class Scene;
using namespace physx;

class ProjectileGenerator
{
public:
	ProjectileGenerator(Scene* s);
	~ProjectileGenerator();

	// para definir la masa simulada Ms = Mr * (Vr^2 / Vs^2)
	// para definir la gravedad simulada Gs = Gr * (Vs^2 / Vr^2)

	// GetCamera()->getDir(); // direccion camera (para adaptar la velocidad de las particulas) => normalizar y multiplicar por el vector velocidad
	// GetCamera()->getEye(); // centro de la camara (posicion para inicializar las particulas)

	void shoot(std::string m);

private:
	int maxBalasCannon = 4;
	int currentBalasCannon = 0;

	Scene* scn = nullptr;
	std::pair<float, Vector3> getSimulationAtributes(float massR, Vector3 gravityR, float vR, float vS);

	std::unordered_map<std::string, Projectile*> projectiles;
	std::string model;
};