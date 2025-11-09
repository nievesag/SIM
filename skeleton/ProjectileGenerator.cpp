#include "ProjectileGenerator.h"
#include "Scene.h"
#include "RenderUtils.hpp"

ProjectileGenerator::ProjectileGenerator(Vector3 Pos, Vector3 Vel, double size, std::string type, Scene* s)
	: scn(s)
{
	Particle* modelo = new Particle(
		scn,							// escena (la misma que el generador)
		Pos,							// origen inicial
		Vel,							// velocidad inicial
		5,								// tamaño
		{ 0.4,0.7,1,1 },	// color
		3,								// masa
		0.99,							// damping
		-1);						// tiempo de vida max
	model = modelo;
	DeregisterRenderItem(modelo->getRenderItem());
}

ProjectileGenerator::~ProjectileGenerator()
{

}

void ProjectileGenerator::shoot()
{
	if (balasCount < balasMax)
	{
		scn->addEntity(new Particle(*model));
		balasCount++;
	}
}

std::pair<float, float> ProjectileGenerator::getSimulationAtributes(float massR, float gravityR, float vR, float vS)
{
	// para definir la masa simulada Ms = Mr * (Vr^2 / Vs^2)
	// para definir la gravedad simulada Gs = Gr * (Vs^2 / Vr^2)

	float massS, gravityS;

	massS = massR * (pow(vR, 2) / pow(vS, 2));
	gravityS = gravityR * (pow(vS, 2) / pow(vR, 2));

	return std::pair<float, float> { massS, gravityR };
}
