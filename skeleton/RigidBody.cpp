#include "RigidBody.h"

// --- STATIC
RigidBodyStatic::RigidBodyStatic(Scene* scn, PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, float siz, Vector4 col)
	: RigidBody(scn)
{
	size = siz;

	PxVec3 volume = { size, size, size };

	color = col;
	pose = new physx::PxTransform(pos);
	actor = gPhysics->createRigidStatic(*pose);
	shape = CreateShape(PxBoxGeometry(volume));
	actor->attachShape(*shape);

	renderItem = new RenderItem(shape, actor, color);
}

// --- DYNAMIC
RigidBodyDynamic::RigidBodyDynamic(Scene* scn, PxPhysics* gPhysics, PxScene* gScene, PxMaterial* mat, 
	bool kin, Vector3 pos, Vector3 vel, double siz, Vector4 col, float m, float damp, float maxLT, 
	Shape sh, double density, PxVec3 angVel, PxVec3 tensor)
	: RigidBody(scn), gScene(gScene), maxLifetime(maxLT), sh(sh), gMaterial(mat), damping(damp)
{
	mass = m;
	size = siz;
	color = col;

	auto volumen = 1;
	switch (sh)
	{
	case BOX:
		shape = CreateShape(PxBoxGeometry(vol), mat);
		volumen = vol.x * vol.y * vol.z;
		break;

	case SPHERE:
		shape = CreateShape(PxSphereGeometry(vol.x), mat);
		volumen = pow(vol.x, 3) * 4 / 3 * PxPi;
		size = vol.x; // radio de la esfera
		break;

	case CAPSULE:
		shape = CreateShape(PxCapsuleGeometry(vol.x, vol.y), mat);
		volumen = vol.x * vol.y * vol.z;
		break;
	}

	pose = new PxTransform(pos);
	actor = gPhysics->createRigidDynamic(*pose);
	//actor->userData = static_cast<void*>(this);
	actor->attachShape(*shape);
	renderItem = new RenderItem(shape, actor, color);

	if (d <= 0) density = m / volumen;
	else  density = d;

	if (tensor != PxVec3(1))
	{
		actor->setMass(mass);
		actor->setMassSpaceInertiaTensor(vol);
	}
	else  PxRigidBodyExt::updateMassAndInertia(*actor, density);

	//setKinematic(kin);
	if (!kin)
	{
		actor->setLinearVelocity(vel);
		actor->setAngularVelocity(angVel);
	}
}