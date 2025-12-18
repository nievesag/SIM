#pragma once
#include "Particle.h"

class Projectile : public Particle
{
public:
	Projectile(Scene* scn, Vector3 pos, Vector3 vel, double size);
	Projectile(Scene* scn, Vector3 pos, Vector3 vel, double siz, Vector4 col, float m, float damp, float maxLT);
	Projectile(const Projectile& model); // constructora con modelo

private:
	virtual void step(double t) override;

	void manageLife(double t) override;
};