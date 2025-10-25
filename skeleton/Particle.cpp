#include "Particle.h"

Particle::Particle(Scene* scn, Vector3 Pos, Vector3 Vel, double siz, Vector4 color, float m, double damp) :
	Entity(scn), damping(damp)
{
	// construye la particula
	pose = new PxTransform(Pos);
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(siz)), pose, color);
	setVelocity(Vel);
	setMass(m);
}

Particle::Particle(Scene* scn, Vector3 Pos, Vector3 Vel, double size) :
	Entity(scn)
{
	// construye la particula
	pose = new PxTransform(Pos);
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(2)), pose, {1,1,1,1});
	setVelocity(Vel);
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

// actualiza su posicion
void Particle::step(double t)
{
	// ---- Fuerzas ----
	applyForce();

	// ---- Integrate ----
	integrate(t);
}

void Particle::applyForce()
{
	// calculamos la fuerza acumulada
	Vector3 totalForc = { 0,0,0 }; // ponemos a 0, porque las fuerzas son instantaneas

	for (auto f : resultingForce)
	{
		totalForc += f;
	}

	resultingForce.clear();

	// F=m*a
	acc = totalForc / mass;

	// Aplica la gravedad si es un objeto con gravedad
	acc += gravity;
}

void Particle::integrate(double t)
{
	// -- Euler explicito
	//pose->p += vel * t;
	//vel += acc * t;

	// -- Euler semi implicito
	vel += acc * t;
	pose->p += vel * t;

	// Damping despues de la integracion (v=v*d^t)
	vel *= pow(damping, t);
}