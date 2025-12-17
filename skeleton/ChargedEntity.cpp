#include "ChargedEntity.h"

ChargedEntity::ChargedEntity(Scene* scn, Vector3 pos, float size, float q, TrailGenerator* _trailGenerator, PxPhysics* gPhysics, PxScene* pxScn)
	: RigidBodyDynamic(scn, gPhysics, pxScn, nullptr, false, pos, {0,0,0}, size,
		{3,3,3}, { 0, 0.95f, 0.81f ,1}, 3, 0.8, -1, SPHERE, -1, 
		{ 0, 0, 0 }, { 1, 1, 1 }, ALL), trailGenerator(_trailGenerator)
{
	setq(q);
	//actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
	setActorFlag();
}

ChargedEntity::~ChargedEntity()
{
	//delete trailGenerator;
}

void ChargedEntity::step(double t)
{
	if (trailGenerator != nullptr) 
	{
		trailGenerator->setPosition(getPosition());
		trailGenerator->setDirection(getRotation().rotate(physx::PxVec3(-1, 0, 0)));
		trailGenerator->setFatherSize(getSize());
	}

	RigidBodyDynamic::step(t);

	//if (trailGenerator != nullptr)
	//{
	//	trailGenerator->setFatherAlive(getAlive());
	//}

	if (getPosition().y < -200)
	{
		//std::cout << "hola" << std::endl;
		getScene()->splash(getPosition());
	}
}