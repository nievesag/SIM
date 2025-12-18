#include "Win.h"

Win::Win(Scene* scn, float size, Vector3 pos, physx::PxPhysics* gPhysics, physx::PxScene* pxScn)
	: RigidBodyDynamic(scn, gPhysics, pxScn, gPhysics->createMaterial(1.0f, 1.0f, 0.01f), true, pos, { 0, 0, 0 },
		size, { size,size,size }, { 1, 0.267, 0.91, 1.0f }, 10, 0.8, -1, CAPSULE, -1, { 0,0,0 }, { 1,1,1 }, WIN)
{

}