#include "ParticleGenerator.h"
#include "Scene.h"

ParticleGenerator::ParticleGenerator(Scene* s)
	: scn(s)
{
	// --- crear modelos de particulas
	// - base
	Particle* pBase = new Particle({0,0,0}, {100, -9.8, 100},5, {1,0,0,1}, 20, 0.99);
	particles.emplace(std::make_pair(std::string("Base"), pBase));
}

ParticleGenerator::~ParticleGenerator()
{
	particles.clear();
}

void ParticleGenerator::step(double t)
{
	
}

void ParticleGenerator::generateParticle(std::string model)
{
	auto it = particles.find(model);

	if (it != particles.end()) 
	{
		//x = x + g(m,t)
		//y = y + g(m,t)
		//z = z + g(m,t)
		//vx = vx + g(m,t)
		//vy = vy + g(m,t)
		//vz = vz + g(m,t)

		Particle* p = new Particle( it->second->getPosition(),	// posicion
									it->second->getVelocity(),	// velocidad
									it->second->getSize(),		// tamano
									it->second->getColor(),		// color
									it->second->getMass(),		// masa
									it->second->getDamping());	// damping



		scn->addEntity(p);
	}
}