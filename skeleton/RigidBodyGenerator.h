#pragma once

#include <PxPhysicsAPI.h>
#include <random>
#include <unordered_map>
#include <vector>
#include "RigidBody.h"

class RigidBodyDynamic;
class Scene;

using namespace physx;

class RigidBodyGenerator
{
public:
	RigidBodyGenerator(Scene* s, std::string mod, PxPhysics* gphys, PxScene* gscn);
	virtual ~RigidBodyGenerator();

	// updates
	virtual void step(double t);
	std::vector<RigidBodyDynamic*> getRbs() { return generatedRb; }

	// --- distribuciones estadisticas (uniforme/normal) -> posicion y velocidad siguen la distribucion
	// uniforme -> genero un rango (min,max) de distintos valores y la distribucion puedo tomar con la misma probabilidad cualquiera de los valores
	// normal -> campana de gauss, valor intermedio (media) segun te alejes de la media hay menos probabilidad, cuando mayor sea la desviacion tipica menos valores habrá cerca de la media (se aplana la curva)
	// std::mt1993t_mt; -> tipo de variable, genera un numero aleatorio entre 0 y 1 cada vez q lo llames
	//std::uniform_real_distribution<double> u{ 0,1 }; => llamar a u(m,t)
	//std::normal_distribution<double> g(0, 1); => llamar a g(m,t)

	// genera la particula
	virtual void generateRb() {};

	void addEntity(RigidBodyDynamic* p);

	void deleteEntities();

	void toggleGeneration() { isActive = !isActive; }

protected:
	// modelo de particula (tipo de particulas que genera el generador)
	// mapa de modelos <nombre del modelo, particula modelo>
	std::unordered_map<std::string, RigidBodyDynamic*> rbs;
	std::string model;
	Scene* scn = nullptr;
	PxPhysics* gphys;
	PxScene* gscn;

	// generador para usar las distribuciones
	std::random_device rd{};
	std::mt19937 generator{ rd() };

	std::vector<RigidBodyDynamic*> generatedRb; // guardo lo que voy generando

	float lifetime; // tiempo que las particulas de este generador pueden estar vivas

	int maxParticles = 0; // particulas maximas que puede generar este generador
	int numParticles = 0; // particulas generadas por cada generador

	bool isActive = true;
};

// ------- GENERADOR CON CARGA -------
class ChargedRbGenerator : public RigidBodyGenerator
{
public:
	ChargedRbGenerator(Scene* s, std::string model, PxPhysics* gphys, PxScene* gscn)
		: RigidBodyGenerator(s, model, gphys, gscn)
	{
		maxParticles = 1;
	}

	~ChargedRbGenerator() override = default;

private:
	void generateRb() override {}
};
