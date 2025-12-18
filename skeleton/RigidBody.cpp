#include "RigidBody.h"
#include "Scene.h"
#include <iostream>

// --- BASE
RigidBody::RigidBody(Scene* scn, physx::PxPhysics* gPhysics, physx::PxScene* gScene, Shape s)
	: Entity(scn), gScene(gScene), sh(s) {}

// --- STATIC
RigidBodyStatic::RigidBodyStatic(Scene* scn, physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 pos, float siz, Vector4 col, Shape s)
	: RigidBody(scn, gPhysics, gScene, s)
{
	size = siz;

	Vector3 volume = { size, size, size };

	color = col;
	pose = new physx::PxTransform(pos);
	actor = gPhysics->createRigidStatic(*pose);
	shape = CreateShape(physx::PxBoxGeometry(volume));

	setGroup(ALL);

	actor->attachShape(*shape);

	renderItem = new RenderItem(shape, actor, color);
	gScene->addActor(*actor);
}

void RigidBodyStatic::setGroup(physx::PxU32 g, bool autoexcluding)
{
	// mutex
	gScene->lockWrite();

	PxFilterData filterData;
	filterData.word0 = g;                 // a qué grupo pertenezco
	filterData.word1 = autoexcluding
		? ~g                              // colisiono con todo menos conmigo
		: ALL;                      // colisiono con todo

	shape->setSimulationFilterData(filterData);

	gScene->unlockWrite();
	group = g;
}

// --- DYNAMIC
RigidBodyDynamic::RigidBodyDynamic(Scene* scn, physx::PxPhysics* gPhysics, physx::PxScene* gScene, physx::PxMaterial* mat, 
	bool kin, Vector3 pos, Vector3 vel, float siz, physx::PxVec3 vol, Vector4 col, float m, float damp, float maxLT,
	Shape s, double d, physx::PxVec3 angVel, physx::PxVec3 tensor, PxU32 group)
	: RigidBody(scn, gPhysics, gScene, s), maxLifetime(maxLT), gMaterial(mat), damping(damp)
{
	mass = m;
	size = siz;
	color = col;

	auto volumen = 1;
	switch (sh)
	{
	case BOX:
		shape = CreateShape(physx::PxBoxGeometry(vol), mat);
		volumen = vol.x * vol.y * vol.z;
		break;

	case SPHERE:
		shape = CreateShape(physx::PxSphereGeometry(vol.x), mat);
		volumen = pow(vol.x, 3) * 4 / 3 * physx::PxPi;
		size = vol.x; // radio de la esfera
		break;

	case CAPSULE:
		shape = CreateShape(physx::PxCapsuleGeometry(vol.x, vol.y), mat);
		volumen = vol.x * vol.y * vol.z;
		break;
	}

	pose = new physx::PxTransform(pos);
	actor = gPhysics->createRigidDynamic(*pose);
	setGroup(group);
	actor->attachShape(*shape);
	actor->userData = static_cast<void*>(this);
	renderItem = new RenderItem(shape, actor, color);

	if (d <= 0) density = m / volumen;
	else  density = d;

	if (tensor != physx::PxVec3(1))
	{
		actor->setMass(mass);
		actor->setMassSpaceInertiaTensor(vol);
	}
	else physx::PxRigidBodyExt::updateMassAndInertia(*actor, density);

	if (!kin)
	{
		actor->setLinearVelocity(vel);
		actor->setAngularVelocity(angVel);
	}
	else
	{
		setKinematic(actor);
	}

	gScene->addActor(*actor);
}

void RigidBodyDynamic::applyForce()
{
	// calculamos la fuerza acumulada
	Vector3 totalForc = { 0,0,0 }; // ponemos a 0, porque las fuerzas son instantaneas

	for (auto f : resultingForce)
	{
		totalForc += f;
	}

	actor->addForce(totalForc);

	resultingForce.clear();
}

void RigidBodyDynamic::setGroup(physx::PxU32 g, bool autoexcluding)
{
	// mutex
	gScene->lockWrite();

	PxFilterData filterData;
	filterData.word0 = g;                 // a qué grupo pertenezco
	filterData.word1 = autoexcluding
		? ~g                              // colisiono con todo menos conmigo
		: ALL;                      // colisiono con todo

	shape->setSimulationFilterData(filterData);

	gScene->unlockWrite();
	group = g;
}

void RigidBodyDynamic::setNoGroup()
{
	setGroup(NONE);
}

void RigidBodyDynamic::collisionCallback()
{
	scene->endLevel();
}

void RigidBodyDynamic::manageLife(double t)
{
	if ((maxLifetime != -1 && lifetime > maxLifetime)	// si tiene vida maxima y se ha pasado
		|| actor->getGlobalPose().p.y <= -scene->getActionThreshold().y	// o se sale por arriba o abajo
		|| actor->getGlobalPose().p.y >= scene->getActionThreshold().y
		|| actor->getGlobalPose().p.x <= -scene->getActionThreshold().x	// o se sale por la der o izq
		|| actor->getGlobalPose().p.x >= scene->getActionThreshold().x
		|| actor->getGlobalPose().p.z <= -scene->getActionThreshold().z	// o se sale por delante o detras
		|| actor->getGlobalPose().p.z >= scene->getActionThreshold().z)
	{
		setAlive(false);
	}

	lifetime += t;
}

void RigidBodyDynamic::addForce(float x, float y, float z)
{
	actor->addForce({ x, y, z });
}

void RigidBodyDynamic::addForce(const Vector3& fc)
{
	resultingForce.push_back(fc);
}

void RigidBodyDynamic::step(double t)
{
	// ---- Fuerzas ----
	if (!resultingForce.empty())
	{
		applyForce();
	}

	// ---- Gestion escena ----
	manageLife(t);
}