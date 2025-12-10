#include "Magnet.h"
#include "RigidBody.h"

Magnet::Magnet(Scene* scn, float size, Vector3 pos, Vector4 col, physx::PxPhysics* gPhysics, physx::PxScene* pxScn)
	: RigidBodyStatic(scn, gPhysics, pxScn, pos, size, col)
{
	magneticField = new RenderItem(CreateShape(physx::PxSphereGeometry(size)), pose, { 1,1,1,0.18f });
}