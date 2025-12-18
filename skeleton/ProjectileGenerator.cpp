#include "ProjectileGenerator.h"
#include "Scene.h"
#include "RenderUtils.hpp"

class Projectile;

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
		2);						// tiempo de vida max
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