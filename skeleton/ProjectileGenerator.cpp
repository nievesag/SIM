#include "ProjectileGenerator.h"
#include "Scene.h"
#include "RenderUtils.hpp"

class Projectile;

ProjectileGenerator::ProjectileGenerator(Vector3 Pos, Vector3 Vel, double size, char type, Scene* s)
	: scn(s)
{
	switch (toupper(type))
	{
	case 'H':
	{
			/*
		Particle* b = new Particle(s, Pos, Vel, size);
		std::pair<float, Vector3> const simAtributes = getSimulationAtributes(4, {0,-9.8f,0}, Vel.magnitude(), 50);
		b->setMass(simAtributes.first);
		b->setGravity(simAtributes.second);
		b->setAcc({ b->getGravity() });
		balas.push_back(b);

		Particle* b1 = new Particle(s, Pos, Vel, size);
		std::pair<float, Vector3> const simAtributes1 = getSimulationAtributes(4, { 0,-9.8f,0 }, Vel.magnitude(), 50);
		b1->setMass(simAtributes1.first);
		b1->setGravity(simAtributes1.second);
		b1->setAcc({ b1->getGravity()});
		balas.push_back(b1);

		Particle* b2 = new Particle(s, Pos, Vel, size);
		std::pair<float, Vector3> const simAtributes2 = getSimulationAtributes(4, { 0,-9.8f,0 }, Vel.magnitude(), 50);
		b2->setMass(simAtributes1.first);
		b2->setGravity(simAtributes1.second);
		b2->setAcc({ b2->getGravity() });
		balas.push_back(b2);
		*/

		break;
	}
	default:
		break;
	}
}

ProjectileGenerator::ProjectileGenerator(Scene* s, std::string mod)
{
	// --- crear modelos de proyectiles
	// - cannon
	Vector3 cameraPos = GetCamera()->getEye();
	Vector3 cameraDir = GetCamera()->getDir().getNormalized();
	Vector3 cameraProjVel = cameraDir * 100;
	Projectile* modeloCannon = new Projectile(s, cameraPos, cameraProjVel, 5);
		std::pair<float, Vector3> const simAtributes = getSimulationAtributes(4, { 0,-9.8f,0 }, cameraProjVel.magnitude(), 50);
		modeloCannon->setMass(simAtributes.first);
		modeloCannon->setGravity(simAtributes.second);
		modeloCannon->setAcc({ modeloCannon->getGravity() });
	projectiles.emplace(std::make_pair(std::string("Cannon"), modeloCannon));
	DeregisterRenderItem(modeloCannon->getRenderItem());
}

ProjectileGenerator::ProjectileGenerator(Scene* s)
	: scn(s)
{
	// --- crear modelos de proyectiles
	// - cannon
	Vector3 cameraPos = GetCamera()->getEye();
	Vector3 cameraDir = GetCamera()->getDir().getNormalized();
	Vector3 cameraProjVel = cameraDir * 100;
	std::pair<float, Vector3> const simAtributes = getSimulationAtributes(4, { 0,-9.8f,0 }, cameraProjVel.magnitude(), 50);

	Projectile* modeloCannon = new Projectile(
		scn,								// escena (la misma que el generador)
		cameraPos,					// origen inicial
		cameraProjVel,				// velocidad inicial
		5,								// tamaño
		{ 1,0,0,1 },		// color  
		simAtributes.first,				// masa
		0.99,							// damping
		-1);						// tiempo de vida max
	modeloCannon->setGravity(simAtributes.second);
	modeloCannon->setAcc({ modeloCannon->getGravity() });

	projectiles.emplace(std::make_pair(std::string("Cannon"), modeloCannon));
	DeregisterRenderItem(modeloCannon->getRenderItem());
}

ProjectileGenerator::~ProjectileGenerator()
{
	projectiles.clear();
}

void ProjectileGenerator::shoot(std::string m)
{
	// buscamos si existe el modelo en el mapa
	auto it = projectiles.find(m);

	if (it != projectiles.end()) // si existe ese modelo...
	{
		if (m == "Cannon")
		{
			if (currentBalasCannon < maxBalasCannon)
			{
				Projectile* p = new Projectile(*it->second);

				Vector3 cameraPos = GetCamera()->getEye();
				Vector3 cameraDir = GetCamera()->getDir().getNormalized();
				Vector3 cameraProjVel = cameraDir * 100;

				p->setPosition(cameraPos);
				p->setVelocity(cameraProjVel);

				scn->addEntity(p);

				currentBalasCannon++;
			}
		}

	}
}

std::pair<float, Vector3> ProjectileGenerator::getSimulationAtributes(float massR, Vector3 gravityR, float vR, float vS)
{
	// para definir la masa simulada Ms = Mr * (Vr^2 / Vs^2)
	// para definir la gravedad simulada Gs = Gr * (Vs^2 / Vr^2)

	float massS;
	Vector3 gravityS;

	massS = massR * (pow(vR, 2) / pow(vS, 2));
	gravityS = gravityR * (pow(vS, 2) / pow(vR, 2));

	return std::pair<float, Vector3> { massS, gravityR };
}