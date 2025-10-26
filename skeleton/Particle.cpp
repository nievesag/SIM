#include "Particle.h"

Particle::Particle(Scene* scn, Vector3 pos, Vector3 vel, double siz, Vector4 col, float m, double damp) :
	Entity(scn), damping(damp)
{
	// construye la particula
	pose = new PxTransform(pos);
	size = siz;
	shape = CreateShape(PxSphereGeometry(size));
	color = col;
	renderItem = new RenderItem(shape, pose, color);
	mass = m;
	velocity = vel;
}

Particle::Particle(Scene* scn, Vector3 Pos, Vector3 Vel, double size) :
	Entity(scn)
{
	// construye la particula
	pose = new PxTransform(Pos);
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(2)), pose, {1,1,1,1});
	setVelocity(Vel);
}

Particle::Particle(const Particle& model) :
	Entity(model.scene)
{
	// crea nueva particula a traves de los atributos de una particula modelo
	pose = new PxTransform(model.pose->p);
	velocity = model.velocity;
	acc = model.acc;
	damping = model.damping;
	size = model.size;
	shape = model.shape;
	color = model.color;
	renderItem = new RenderItem(shape, pose, color);
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
	velocity += acc * t;
	pose->p += velocity * t;

	// Damping despues de la integracion (v=v*d^t)
	velocity *= pow(damping, t);
}