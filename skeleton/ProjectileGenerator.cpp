#include "ProjectileGenerator.h"
#include "Scene.h"

ProjectileGenerator::ProjectileGenerator(Vector3 Pos, Vector3 Vel, double size, char type, Scene* s)
	: scn(s)
{
	switch (toupper(type))
	{
	case 'H':
	{
		Particle* b = new Particle(Pos, Vel, size);
		std::pair<float, float> const simAtributes = getSimulationAtributes(4,9.8,Vel.magnitude(),50);
		b->setMass(simAtributes.first);
		b->setGravity(simAtributes.second);
		b->setAcc({ 0,-b->getGravity(),0 });
		balas.push_back(b);

		Particle* b1 = new Particle(Pos, Vel, size);
		std::pair<float, float> const simAtributes1 = getSimulationAtributes(4, 9.8, Vel.magnitude(), 50);
		b->setMass(simAtributes1.first);
		b->setGravity(simAtributes1.second);
		b->setAcc({ 0,-b->getGravity(),0 });

		balas.push_back(b1);

		break;
	}
	default:
		break;
	}
}

ProjectileGenerator::~ProjectileGenerator()
{

}

void ProjectileGenerator::shoot(char type)
{
	switch (toupper(type))
	{
	case 'H':
	{
		if (!balas.empty()) 
		{
			scn->addEntity(*balas.begin());
			balas.erase(balas.begin());
		}
		
		break;
	}
	default:
		break;
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
