#pragma once

#include <PxPhysicsAPI.h>

#include <vector>
#include <unordered_map>

#include "core.hpp"
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
	virtual void step(double t);
	std::vector<Particle*> getParticles() { return generatedParticles; }

	// --- distribuciones estadisticas (uniforme/normal) -> posicion y velocidad siguen la distribucion
	// uniforme -> genero un rango (min,max) de distintos valores y la distribucion puedo tomar con la misma probabilidad cualquiera de los valores
	// normal -> campana de gauss, valor intermedio (media) segun te alejes de la media hay menos probabilidad, cuando mayor sea la desviacion tipica menos valores habrá cerca de la media (se aplana la curva)
	// std::mt1993t_mt; -> tipo de variable, genera un numero aleatorio entre 0 y 1 cada vez q lo llames
	//std::uniform_real_distribution<double> u{ 0,1 }; => llamar a u(m,t)
	//std::normal_distribution<double> g(0, 1); => llamar a g(m,t)

	// genera la particula
	virtual void generateParticle() = 0;

	void deleteEntities();

	void toggleGeneration() { isActive = !isActive; }

protected:
	// modelo de particula (tipo de particulas que genera el generador)
	// mapa de modelos <nombre del modelo, particula modelo>
	std::unordered_map<std::string, Particle*> particles;
	std::string model;
	Scene* scn = nullptr;

	// generador para usar las distribuciones
	std::random_device rd{};
	std::mt19937 generator{ rd() };

	std::vector<Particle*> generatedParticles; // guardo lo que voy generando

	float lifetime; // tiempo que las particulas de este generador pueden estar vivas

	int maxParticles = 0; // particulas maximas que puede generar este generador
	int numParticles = 0; // particulas generadas por cada generador

	bool isActive = true;
};

// ------- GENERADOR CASACADA -------
class WaterfallGenerator : public ParticleGenerator
{
public:
	WaterfallGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model) {}

	~WaterfallGenerator() override = default;
	void setOrigin(Vector3 ori) { origin = ori; }

private:
	void generateParticle() override;
	Vector3 origin = {0,0,0};
};

// ------- GENERADOR NIEBLA -------
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

// ------- GENERADOR FUEGOS -------
class FireworkGenerator : public ParticleGenerator
{
public:
	FireworkGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model) {}

	~FireworkGenerator() override = default;

private:
	void generateParticle() override;
};

// ------- GENERADOR SPLASH -------
class SplashGenerator : public ParticleGenerator
{
public:
	SplashGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model) {}

	~SplashGenerator() override = default;
	void setSplasPos(Vector3 s) { splashPos = s; }

	void generateParticle() override;

	void setSplash(bool s) { isSplashing = s; }

private:
	Vector3 splashPos = { 0,0,0 };
	bool isSplashing = false;
};

// ------- GENERADOR DISTINTAS MASAS -------
class RandomMassGenerator : public ParticleGenerator
{
public:
	RandomMassGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model) {}

	~RandomMassGenerator() override = default;

	void setXpos(float x) { xPos = x; }
	void setOrigin(Vector3 ori) { origin = ori; }

private:
	void generateParticle() override;

	Vector3 origin{ 0,0,0 };
	float xPos = 0;
};

// ------- GENERADOR CON CARGA -------
class ChargedGenerator : public ParticleGenerator
{
public:
	ChargedGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model)
	{
		maxParticles = 1;
	}

	~ChargedGenerator() override = default;

private:
	void generateParticle() override {}
};

// ------- GENERADOR CON MUELLE -------
class SpringParticleGenerator : public ParticleGenerator
{
public:
	SpringParticleGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model)
	{

	}

	~SpringParticleGenerator() override = default;
	void addSpringEnitity(Particle* p);

private:
	void generateParticle() override {};
};

// ------- GENERADOR RASTRO -------
class TrailGenerator : public ParticleGenerator
{
public:
	TrailGenerator(Scene* s, std::string model)
		: ParticleGenerator(s, model) {}

	~TrailGenerator() override = default;

	void setFatherPart(Entity* fp) { fatherPart = fp; }

	//virtual void step(double t) override;

	Vector3 getPosition() const { return position; }
	Vector3 getDirection() const { return direction; }

	void setPosition(Vector3 p) { position = p; }
	void setDirection(Vector3 d) { direction = d; }
	void setFatherSize(float s) { size = s; }
	void setFatherAlive(bool a) { fatherAlive = a; }

private:
	void generateParticle() override;
	Entity* fatherPart = nullptr; // estas particulas siguen a una padre

	Vector3 position;
	Vector3 direction;
	float size;
	bool fatherAlive = true;
};