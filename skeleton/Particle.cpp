#include "Particle.h"

#include "Scene.h"

Particle::Particle(Scene* scn, Vector3 pos, Vector3 vel, double siz, Vector4 col, float m, float damp, float maxLT) :
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
	lifetime = 0;
	maxLifetime = maxLT;
}

/*
Particle::Particle(Scene* scn, Vector3 pos, Vector3 vel, double siz, Vector4 col, float m, float damp, float maxLT, physx::PxShape* s)
	: Entity(scn), damping(damp)
{
	// construye la particula
	pose = new PxTransform(pos);
	size = siz;
	shape = s;
	color = col;
	renderItem = new RenderItem(shape, pose, color);
	mass = m;
	velocity = vel;
	lifetime = 0;
	maxLifetime = maxLT;
}
*/

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
	mass = model.mass;
	lifetime = model.lifetime;
	maxLifetime = model.maxLifetime;
}

Particle::Particle(const Particle& model, float newSize)
	: Entity(model.scene)
{
	// crea nueva particula a traves de los atributos de una particula modelo con variacion de tamaño
	pose = new PxTransform(model.pose->p);
	velocity = model.velocity;
	acc = model.acc;
	damping = model.damping;
	size = newSize;
	shape = CreateShape(PxSphereGeometry(newSize));
	color = model.color;
	renderItem = new RenderItem(shape, pose, color);
	mass = model.mass;
	lifetime = model.lifetime;
	maxLifetime = model.maxLifetime;
}

Particle::~Particle()
{
	//DeregisterRenderItem(renderItem);
}

// actualiza su posicion
void Particle::step(double t)
{
	// ---- Fuerzas ----
	applyForce();

	// ---- Integrate ----
	integrate(t);

	// ---- Gestion escena ----
	manageLife(t);
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

	acc = totalForc / mass; // a = F/a

	// aplica la gravedad
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

void Particle::manageLife(double t)
{
	if ((maxLifetime != -1 && lifetime > maxLifetime)	// si tiene vida maxima y se ha pasado
		|| pose->p.y <= -scene->getActionThreshold().y	// o se sale por arriba o abajo
		|| pose->p.y >= scene->getActionThreshold().y
		|| pose->p.x <= -scene->getActionThreshold().x	// o se sale por la der o izq
		|| pose->p.x >= scene->getActionThreshold().x 
		|| pose->p.z <= -scene->getActionThreshold().z	// o se sale por delante o detras
		|| pose->p.z >= scene->getActionThreshold().z)
	{
		setAlive(false);
	}

	lifetime += t;
}
