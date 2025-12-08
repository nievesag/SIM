#include "RigidBody.h"

RigidBodyStatic::RigidBodyStatic(Scene* scn, PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, float size, Vector4 color)
	: RigidBody(scn)
{
	PxVec3 volume = { size, size, size };

	pose = new physx::PxTransform(pos);
	actor = gPhysics->createRigidStatic(*pose);
	shape = CreateShape(PxBoxGeometry(volume));
	actor->attachShape(*shape);

	renderItem = new RenderItem(shape, actor, { 0.7, 0.7, 0.7, 1 });
}
