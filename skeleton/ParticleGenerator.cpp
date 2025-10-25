#include "ParticleGenerator.h"

#include "Scene.h"

ParticleGenerator::ParticleGenerator(Scene* s, std::string model)
	: scn(s)
{
	// --- crear modelos de particulas
	// - base
	Particle* pBase = new Particle(scn, {0,0,0}, {100, -9.8, 100},5, {1,0,0,1}, 20, 0.99);
	particles.emplace(std::make_pair(std::string("Base"), pBase));
}

ParticleGenerator::~ParticleGenerator()
{
	particles.clear();
}

void ParticleGenerator::step(double t)
{
	generateParticle(); // creacion de las particulas
	std::cout << "pim particle" << std::endl;
}


// ------- GENERADOR CASACADA -------
void WaterfallGenerator::generateParticle()
{
	// buscamos si existe el modelo en el mapa
	auto it = particles.find(model);

	if (it != particles.end()) // si existe ese modelo...
	{
		// distribuciones
		std::uniform_int_distribution<> particlesToGenerateDistr(0, 10); 
		std::normal_distribution<> velYDistr(5, 2.0);	// media|dispersion
		std::normal_distribution<> velZDistr(10, 2.0);	// media|dispersion
		std::normal_distribution<> posXDistr(0, 10.0);	// media|dispersion
		std::normal_distribution<> posZDistr(0, 10.0);	// media|dispersion

		
		int particlesToGenerate = particlesToGenerateDistr(generator); // particulas que se generaran en este tick

		for (int i = 0; i < particlesToGenerate; i++) // genero las particulas que sean
		{
			Vector3 newOrg;	// posicion en la que se genera
			Vector3 newVel;	// velocidad con la que se genera

			// org
			newOrg.x = posXDistr(generator);
			newOrg.y = 0;
			newOrg.z = posZDistr(generator);
			// vel
			newVel.x = 0;
			newVel.y = velZDistr(generator);
			newVel.z = velYDistr(generator);

			// creamos la nueva particula
			Particle* p = new Particle(scn,			// escena
					newOrg,						// posicion
					newVel,						// velocidad
					it->second->getSize(),			// tamano
					it->second->getColor(),		// color
					it->second->getMass(),			// masa
				it->second->getDamping());		// damping

			generatedParticles.push_back(p);
			scn->addEntity(p);
		}
	}
}