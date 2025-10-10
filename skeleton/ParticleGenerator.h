#pragma once

#include <PxPhysicsAPI.h>

#include <vector>
#include <unordered_map>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Particle.h"

class Scene;
using namespace physx;

class ParticleGenerator
{
public:
	ParticleGenerator(Scene* s, std::string model);
	~ParticleGenerator();

	// update
	void step(double t);

	// --- distribuciones estadisticas (uniforme/normal) -> posicion y velocidad siguen la distribucion
	// uniforme -> genero un rango (min,max) de distintos valores y la distribucion puedo tomar con la misma probabilidad cualquiera de los valores
	// normal -> campana de gauss, valor intermedio (media) segun te alejes de la media hay menos probabilidad, cuando mayor sea la desviacion tipica menos valores habrá cerca de la media (se aplana la curva)
	// std::mt1993t_mt; -> tipo de variable, genera un numero aleatorio entre 0 y 1 cada vez q lo llames
	//std::uniform_real_distribution<double> u{ 0,1 }; => llamar a u(m,t)
	//std::normal_distribution<double> g(0, 1); => llamar a g(m,t)

	// genera la particula
	void generateParticle(std::string m);

private:
	// modelo de particula (atributos) (tipo de particulas que genera el generador)
	// mapa de modelos <nombre del modelo, particula modelo>
	std::unordered_map<std::string, Particle*> particles;
	std::string model;
	Scene* scn = nullptr;
};