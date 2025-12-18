#include "Projectile.h"

#include "Scene.h"

Projectile::Projectile(Scene* scn, Vector3 pos, Vector3 vel, double size)
	: Particle(scn, pos, vel, size)
{
}

Projectile::Projectile(Scene* scn, Vector3 pos, Vector3 vel, double siz, Vector4 col, float m, float damp, float maxLT)
	: Particle(scn, pos, vel, siz, col, m, damp, maxLT)
{

}

Projectile::Projectile(const Projectile& model)
	: Particle(model)
{

}

void Projectile::step(double t)
{
	//Particle::step(t);
	integrate(t);
	manageLife(t);

	std::cout << maxLifetime << " " << lifetime << std::endl;
}

void Projectile::manageLife(double t)
{
	if ((maxLifetime != -1 && lifetime > maxLifetime))	// si tiene vida maxima y se ha pasado
	{
		scene->explosion(getPosition());

		setAlive(false);
	}

	lifetime += t;
}
