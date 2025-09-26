#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel, double size) :
	vel(Vel)
{
	// construye la particula
	pose = new PxTransform(Pos);
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(size)), pose, { 1,1,1,1 });
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

// actualiza su posicion
void Particle::integrate(double t)
{
	// Euler explicito
	//pose->p += vel * t;
	//vel += acc * t;

	// Euler semi implicito
	vel += acc * t;
	pose->p += vel * t;

	// damping despues de la integracion (v=v*d^t)
	vel *= pow(damping, t);
}
