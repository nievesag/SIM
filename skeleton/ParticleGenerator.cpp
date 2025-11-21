#include "ParticleGenerator.h"

#include <cassert>

#include "Scene.h"

/// <summary>
/// En la constructura se construyen las particulas modelos
/// MODELOS:
/// - Cascada
/// - Niebla
/// - Fuegos
/// - Carga
/// - Rastro
/// [Estos strings definen a los modelos]
/// </summary>
ParticleGenerator::ParticleGenerator(Scene* s, std::string mod)
	: scn(s), model(mod)
{
	// --- crear modelos de particulas
	// - cascada
	Particle* modeloWaterfall = new Particle(
		scn,							// escena (la misma que el generador)
		Vector3(0, 0, 0),	// origen inicial
		{0, 0, 0},	// velocidad inicial
		5,								// tamaño
		{0.4,0.7,1,1},	// color  
		3,								// masa
		0.99,							// damping
		-1);						// tiempo de vida max
	particles.emplace(std::make_pair(std::string("Cascada"), modeloWaterfall));
	DeregisterRenderItem(modeloWaterfall->getRenderItem());

	// - niebla
	Particle* modeloMist = new Particle(
		scn,								// escena (la misma que el generador)
		Vector3(0, 0, 0),		// origen inicial
		{ 0, 0, 0 },				// velocidad inicial
		15,									// tamaño
		{ 0.5,0.6,0.7,1 },	// color
		0.01,								// masa
		0.99,								// damping
		3);							// tiempo de vida max
	particles.emplace(std::make_pair(std::string("Niebla"), modeloMist));
	DeregisterRenderItem(modeloMist->getRenderItem());

	// - fuego artificial
	Particle* modeloFirework = new Particle(
		scn,							// escena (la misma que el generador)
		Vector3(0, 0, 0),	// origen inicial
		{ 100, -9.8, 100 },	// velocidad inicial
		5,								// tamaño
		{ 0,1,0,1 },		// color
		6,								// masa
		0.99,							// damping
		3);						// tiempo de vida max
	particles.emplace(std::make_pair(std::string("Fuegos"), modeloFirework));
	DeregisterRenderItem(modeloFirework->getRenderItem());

	// - particula cargada
	Particle* modeloCharge = new Particle(
		scn,							// escena (la misma que el generador)
		Vector3(0, 0, 0),	// origen inicial
		{ 100, -9.8, 100 }, // velocidad inicial
		5,								// tamaño
		{ 0,1,0,1 },		// color
		3,								// masa
		0.99,							// damping
		-1);						// tiempo de vida max
	modeloCharge->setq(2);
	particles.emplace(std::make_pair(std::string("Carga"), modeloCharge));
	DeregisterRenderItem(modeloCharge->getRenderItem());

	// - particula rastro
	Particle* modeloTrail = new Particle(
		scn,								// escena (la misma que el generador)
		Vector3(0, 0, 0),					// origen inicial
		{ 0, 0, 0 },						// velocidad inicial
		1,									// tamaño
		{ 1, 0.984, 0, 1 },					// color
		1,									// masa
		0.99,								// damping
		0.2);								// tiempo de vida max
	particles.emplace(std::make_pair(std::string("Rastro"), modeloTrail));
	DeregisterRenderItem(modeloTrail->getRenderItem());

	Particle* modeloSplash = new Particle(
		scn,								// escena (la misma que el generador)
		Vector3(0, 0, 0),		// origen inicial
		{ 0, 0, 0 },				// velocidad inicial
		1,									// tamaño
		{ 1,1,1,1 },			// color
		1,									// masa
		0.99,								// damping
		0.2);							// tiempo de vida max
	particles.emplace(std::make_pair(std::string("Splash"), modeloSplash));
	DeregisterRenderItem(modeloSplash->getRenderItem());
}

ParticleGenerator::~ParticleGenerator()
{
	particles.clear();
}

void ParticleGenerator::step(double t)
{
	deleteEntities();
	generateParticle(); // creacion de las particulas
}

void ParticleGenerator::deleteEntities()
{
	for (auto p : generatedParticles)
	{
		if (!p->getAlive())
		{
			auto it = std::find(generatedParticles.begin(), generatedParticles.end(), p);

			if (it != generatedParticles.end())
			{
				generatedParticles.erase(it);
			}
		}
	}
}

// ------- GENERADOR CASACADA -------
void WaterfallGenerator::generateParticle()
{
	// buscamos si existe el modelo en el mapa
	auto it = particles.find(model);

	if (it != particles.end()) // si existe ese modelo...
	{
		// distribuciones
		std::uniform_int_distribution<> particlesToGenerateDistr(1, 10); 
		std::normal_distribution<> velYDistr(5, 2.0);	// normal(media,desviacion tipica)
		std::normal_distribution<> velZDistr(5, 2.0);
		std::normal_distribution<> posXDistr(0, 10.0);

		int particlesToGenerate = particlesToGenerateDistr(generator); // particulas que se generaran en este tick

		for (int i = 0; i < particlesToGenerate; i++) // genero las particulas que sean
		{
			Vector3 newOrg;	// posicion en la que se genera
			Vector3 newVel;	// velocidad con la que se genera

			// org
			newOrg.x = posXDistr(generator); // solo en la x, efecto cascada
			newOrg.y = it->second->getPosition().y;
			newOrg.z = it->second->getPosition().z;
			// vel
			newVel.x = 0;
			newVel.y = velZDistr(generator);
			newVel.z = velYDistr(generator);

			// creamos la nueva particula
			Particle* p = new Particle(*it->second);

			p->setPosition(-newOrg);
			p->setVelocity(newVel);

			p->setq(it->second->getq());

			generatedParticles.push_back(p);
			scn->addEntity(p);
		}
	}
	else
	{
		std::cout << "[!] NO EXISTE MODELO: " << model << std::endl;
	}
}

// ------- GENERADOR NIEBLA -------
void MistGenerator::generateParticle()
{
	// buscamos si existe el modelo en el mapa
	auto it = particles.find(model);

	if (it != particles.end()) // si existe ese modelo...
	{
		// distribuciones
		std::uniform_int_distribution<> particlesToGenerateDistr(1, 2);
		std::normal_distribution<> posXDistr(-10.0, 10.0);
		std::normal_distribution<> posYDistr(0, 20.0);
		std::normal_distribution<> posZDistr(-10.0, 10.0);

		int particlesToGenerate = particlesToGenerateDistr(generator); // particulas que se generaran en este tick

		for (int i = 0; i < particlesToGenerate; i++)
		{
			Vector3 newOrg;	// posicion en la que se genera
			Vector3 newVel;	// velocidad con la que se genera

			// org
			newOrg.x = posXDistr(generator);
			newOrg.y = posYDistr(generator);
			newOrg.z = posZDistr(generator);
			// vel
			newVel.x = 0;
			newVel.y = 0;
			newVel.z = 0;

			// creamos la nueva particula
			Particle* p = new Particle(*it->second);

			p->setPosition(newOrg);
			p->setVelocity(newVel);

			generatedParticles.push_back(p);
			scn->addEntity(p);
		}
	}
	else
	{
		std::cout << "[!] NO EXISTE MODELO: " << model << std::endl;
	}
}

// ------- GENERADOR FUEGOS ARTIFICIALES -------
void FireworkGenerator::generateParticle()
{
	// buscamos si existe el modelo en el mapa
	auto it = particles.find(model);

	if (it != particles.end()) // si existe ese modelo...
	{
		// distribuciones
		std::uniform_int_distribution<> particlesToGenerateDistr(1, 10);
		std::normal_distribution<> velXDistr(0, 10.0);
		std::normal_distribution<> velYDistr(0, 10.0);
		std::normal_distribution<> velZDistr(0, 10.0);

		int particlesToGenerate = particlesToGenerateDistr(generator); // particulas que se generaran en este tick

		for (int i = 0; i < particlesToGenerate; i++)
		{
			Vector3 newOrg;	// posicion en la que se genera
			Vector3 newVel;	// velocidad con la que se genera

			// org
			newOrg.x = 0;
			newOrg.y = 0;
			newOrg.z = 0;
			// vel
			newVel.x = velXDistr(generator);
			newVel.y = velYDistr(generator);
			newVel.z = 0; // 2D

			// creamos la nueva particula
			Particle* p = new Particle(*it->second);

			p->setPosition(newOrg);
			p->setVelocity(newVel);

			generatedParticles.push_back(p);
			scn->addEntity(p);
		}
	}
	else
	{
		std::cout << "[!] NO EXISTE MODELO: " << model << std::endl;
	}
}

// ------- GENERADOR SPLASH -------
void SplashGenerator::generateParticle()
{
	// buscamos si existe el modelo en el mapa
	auto it = particles.find(model);

	if (it != particles.end()) // si existe ese modelo...
	{
		if (isSplashing)
		{
			// distribuciones
			std::uniform_int_distribution<> particlesToGenerateDistr(1, 10);
			std::normal_distribution<> velXDistr(0, 20.0);
			std::uniform_int_distribution<> velYDistr(0, 100.0);
			std::normal_distribution<> velZDistr(0, 20.0);

			//int particlesToGenerate = particlesToGenerateDistr(generator); // particulas que se generaran en este tick

			for (int i = 0; i < 90; i++)
			{
				Vector3 newOrg;	// posicion en la que se genera
				Vector3 newVel;	// velocidad con la que se genera

				// org
				newOrg.x = splashPos.x;
				newOrg.y = splashPos.y;
				newOrg.z = splashPos.z;
				// vel
				newVel.x = velXDistr(generator);
				newVel.y = velYDistr(generator);
				newVel.z = velZDistr(generator);

				// creamos la nueva particula
				Particle* p = new Particle(*it->second);

				p->setPosition(newOrg);
				p->setVelocity(newVel);

				generatedParticles.push_back(p);
				scn->addEntity(p);
			}

			isSplashing = false;
		}
	}
	else
	{
		std::cout << "[!] NO EXISTE MODELO: " << model << std::endl;
	}
}

// ------- GENERADOR DISTINTAS MASAS -------
void RandomMassGenerator::generateParticle()
{
	// buscamos si existe el modelo en el mapa
	auto it = particles.find(model);

	if (it != particles.end()) // si existe ese modelo...
	{
		if (isActive)
		{
			std::uniform_int_distribution<> particlesToGenerateDistr(1, 3);
			std::normal_distribution<> posYZDistr(0, 10);
			std::uniform_real_distribution<> lifetimeDistr(1, 3);
			std::uniform_real_distribution<> sizeDistr(2, 5);
			std::uniform_real_distribution<> massDistr(2, 5);
			//std::normal_distribution<> massDistr(20, 10);

			int particlesToGenerate = particlesToGenerateDistr(generator);

			for (int i = 0; i < particlesToGenerate; i++)
			{
				Vector3 newOrg;	// posicion en la que se genera
				Vector3 newVel;	// velocidad con la que se genera
				float size = sizeDistr(generator);
				float mass = massDistr(generator);

				newOrg.x = xPos;
				newOrg.y = posYZDistr(generator);
				newOrg.z = posYZDistr(generator);

				newVel.x = 0;
				newVel.y = 0;
				newVel.z = 0;

				float lifetime = lifetimeDistr(generator);

				// creamos la nueva particula
				Particle* p = new Particle(*it->second);

				p->setPosition(newOrg);
				p->setVelocity(newVel);
				p->setMaxLifetime(lifetime);
				p->setMass(mass);

				generatedParticles.push_back(p);
				scn->addEntity(p);
			}
		}
	}
	else
	{
		std::cout << "[!] NO EXISTE MODELO: " << model << std::endl;
	}
}

// ------- GENERADOR PARTICULAS CARGADAS -------
void ChargedGenerator::addChargedEnitity(ChargedEntity* p)
{
	generatedParticles.push_back(p);
	scn->addEntity(p);
}

// ------- GENERADOR PARTICULAS MUELLEADAS -------
void SpringParticleGenerator::addSpringEnitity(Particle* p)
{
	generatedParticles.push_back(p);
}

// ------- GENERADOR RASTRO -------
void TrailGenerator::generateParticle()
{
	// buscamos si existe el modelo en el mapa
	auto it = particles.find(model);

	if (it != particles.end()) // si existe ese modelo...
	{
		if (fatherAlive)
		{
			// distribuciones
			std::uniform_int_distribution<> particlesToGenerateDistr(1, 2);
			std::normal_distribution<> lifetimeDistr(0.1, 0.2);

			int particlesToGenerate = particlesToGenerateDistr(generator); // particulas que se generaran en este tick

			for (int i = 0; i < particlesToGenerate; i++)
			{
				Vector3 newOrg = position;
				Vector3 newVel = direction;

				// creamos la nueva particula
				Particle* p = new Particle(*it->second);

				p->setPosition(newOrg);
				p->setVelocity(newVel);
				p->setMaxLifetime(lifetimeDistr(generator));

				generatedParticles.push_back(p);
				scn->addEntity(p);
			}
		}
	}
	else
	{
		std::cout << "[!] NO EXISTE MODELO: " << model << std::endl;
	}
}


