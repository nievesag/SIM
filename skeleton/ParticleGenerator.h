#pragma once

#include <PxPhysicsAPI.h>

#include <vector>
#include <unordered_map>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include <random>

#include "Particle.h"

class Scene;
using namespace physx;

class ParticleGenerator
{
public:
	ParticleGenerator(Scene* s, std::string mod);
	virtual ~ParticleGenerator();

	// updates
	void step(double t);
	std::vector<Particle*> getParticles() { return generatedParticles; }

	// --- distribuciones estadisticas (uniforme/normal) -> posicion y velocidad siguen la distribucion
	// uniforme -> genero un rango (min,max) de distintos valores y la distribucion puedo tomar con la misma probabilidad cualquiera de los valores
	// normal -> campana de gauss, valor intermedio (media) segun te alejes de la media hay menos probabilidad, cuando mayor sea la desviacion tipica menos valores habrá cerca de la media (se aplana la curva)
	// std::mt1993t_mt; -> tipo de variable, genera un numero aleatorio entre 0 y 1 cada vez q lo llames
	//std::uniform_real_distribution<double> u{ 0,1 }; => llamar a u(m,t)
	//std::normal_distribution<double> g(0, 1); => llamar a g(m,t)

	// genera la particula
	virtual void generateParticle() = 0;

protected:
	// modelo de particula (atributos) (tipo de particulas que genera el generador)
	// mapa de modelos <nombre del modelo, particula modelo>
	std::unordered_map<std::string, Particle*> particles;
	std::string model;
	Scene* scn = nullptr;

	// generador para usar las distribuciones
	std::random_device rd{};
	std::mt19937 generator{ rd() };

	std::vector<Particle*> generatedParticles; // guardo lo que voy generando

	float lifetime; // tiempo que las particulas de este generador pueden estar vivas

	void deleteEntities();

	void deleteParticle(Particle* p);
};

// ------- GENERADOR CASACADA -------
class WaterfallGenerator : public ParticleGenerator
{
public:
	WaterfallGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model) {}

	~WaterfallGenerator() override = default;

private:
	void generateParticle() override;
};

class MistGenerator : public ParticleGenerator
{
public:
	MistGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model)
	{
		
	}

	~MistGenerator() override = default;

private:
	void generateParticle() override;
};

class FireworkGenerator : public ParticleGenerator
{
public:
	FireworkGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model) {}

	~FireworkGenerator() override = default;

private:
	void generateParticle() override;
};