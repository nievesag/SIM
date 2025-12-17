#include "Magnet.h"
#include "RigidBody.h"

Magnet::Magnet(Scene* scn, float size, Vector3 pos, Vector4 col, physx::PxPhysics* gPhysics, physx::PxScene* pxScn, float magneticFieldR)
	: RigidBodyDynamic(scn, gPhysics, pxScn, gPhysics->createMaterial(1.0f, 1.0f, 0.01f), true, pos, { 0, 0, 0 }, 
		size, {size,size,size}, col, 10, 0.8, -1, BOX, -1, {0,0,0}, {1,1,1}, ALL)
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