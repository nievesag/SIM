#include "Magnet.h"
#include "RigidBody.h"

Magnet::Magnet(Scene* scn, float size, Vector3 pos, Vector4 col, physx::PxPhysics* gPhysics, physx::PxScene* pxScn, float magneticFieldR)
	: RigidBodyStatic(scn, gPhysics, pxScn, pos, size, col)
{
	magneticField = new Entity(scn);

	physx::PxTransform* p = new physx::PxTransform(pos);
	magneticField->setPose(p);
	// area que representa el campo magnetico
	magneticField->setRenderItem(new RenderItem(CreateShape(physx::PxSphereGeometry(magneticFieldR)), p, { 1,1,1,0.18f }));
}

void Magnet::updateMagneticField(Vector3 pos)
{
	magneticField->setPosition(pos);
}
