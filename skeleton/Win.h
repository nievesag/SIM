#pragma once
#include "RigidBody.h"

class Win : public RigidBodyDynamic
{
public:
	Win(Scene* scn,Vector3 pos, physx::PxPhysics* gPhysics, physx::PxScene* pxScn);

private:

};