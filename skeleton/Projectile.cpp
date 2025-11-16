#include "Projectile.h"

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
	integrate(t);
}
