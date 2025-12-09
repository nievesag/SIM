#pragma once
#include "core.hpp"
#include "Scene.h"


class RigidBodyDynamic;

class TrailGenerator;

class ChargedEntity : public RigidBodyDynamic
{
public:
	ChargedEntity(Scene* scn, Vector3 pos, float size, float q, TrailGenerator* _trailGenerator, physx::PxPhysics* gPhysics, physx::PxScene* pxScn);
	~ChargedEntity(); 

	void step(double t) override;

private:
	TrailGenerator* trailGenerator = nullptr;
};