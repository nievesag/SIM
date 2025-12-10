#include "RigidBody.h"

// --- STATIC
RigidBodyStatic::RigidBodyStatic(Scene* scn, PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, float siz, Vector4 col)
	: RigidBody(scn)
{
	size = siz;

	Vector3 volume = { size, size, size };

	color = col;
	pose = new physx::PxTransform(pos);
	actor = gPhysics->createRigidStatic(*pose);
	shape = CreateShape(PxBoxGeometry(volume));
	actor->attachShape(*shape);

	renderItem = new RenderItem(shape, actor, color);
}

// --- DYNAMIC
RigidBodyDynamic::RigidBodyDynamic(Scene* scn, PxPhysics* gPhysics, PxScene* gScene, PxMaterial* mat, 
	bool kin, Vector3 pos, Vector3 vel, float siz, PxVec3 vol, Vector4 col, float m, float damp, float maxLT,
	Shape sh, double d, PxVec3 angVel, PxVec3 tensor)
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
	gScene->addActor(*actor);
	actor->userData = static_cast<void*>(this);
	actor->attachShape(*shape);
	renderItem = new RenderItem(shape, actor, color);

	if (d <= 0) density = m / volumen;
	else  density = d;

	if (tensor != PxVec3(1))
	{
		actor->setMass(mass);
		actor->setMassSpaceInertiaTensor(vol);
	}
	else PxRigidBodyExt::updateMassAndInertia(*actor, density);

	if (!kin)
	{
		actor->setLinearVelocity(vel);
		actor->setAngularVelocity(angVel);
	}
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
		std::cout << "muere" << std::endl;
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

	//std::cout << actor->getGlobalPose().p.y  << std::endl;
	//std::cout << actor->getMass()  << std::endl;
	//std::cout << mass  << std::endl;
	//std::cout << pose->p.x << " " << pose->p.y << " " << pose->p.z << " " << std::endl;
}