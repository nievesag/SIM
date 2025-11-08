#include "ChargedEntity.h"
#include "ParticleGenerator.h"

ChargedEntity::ChargedEntity(Scene* scn, Vector3 pos, float size, float q, TrailGenerator* _trailGenerator)
	: Particle(scn, pos, {0,0,0}, size), trailGenerator(_trailGenerator)
{
	setq(q);
}

ChargedEntity::~ChargedEntity()
{
	//delete trailGenerator;
}

void ChargedEntity::step(double t)
{
	trailGenerator->setPosition(getPosition());
	trailGenerator->setDirection(getRotation().rotate(physx::PxVec3(-1, 0, 0)));
	trailGenerator->setFatherSize(getSize());

	Particle::step(t);

	trailGenerator->setFatherAlive(getAlive());
}