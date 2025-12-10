#include "Magnet.h"
#include "RigidBody.h"

Magnet::Magnet(Scene* scn, float size, Vector3 pos, Vector4 col, PxPhysics* gPhysics, PxScene* pxScn)
	: RigidBodyStatic(scn, gPhysics, pxScn, pos, size, col)
{
    
}
