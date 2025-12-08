#pragma once

#include <PxPhysicsAPI.h>
#include "Scene.h"
#include "Entity.h"

class RigidBody : public Entity
{
	// renderizado y el almacenamiento de atributos que sean relevantes para la gestion
	// Para los sólidos estáticos no hara falta crear una clase, podremos crearlos directamente en el main al inicio de nuestra ejecucion
public:
	RigidBody(Scene* scn) : Entity(scn) {}

	// set/get mass
	// setMassSpaceInertiaTensor
	// set/getLinearVelocity
	// set/getAngularVelocity
	// addForce()
	// addTorque()
	// Attach Shape

	virtual PxActor* getActor() { return nullptr; }

	//virtual void step(double dt) override;
	/*void add_torque(physx::PxVec3 add_t);*/
	//virtual void translate(physx::PxVec3) override;
	//virtual void translate_to(physx::PxVec3) override;
	
protected:
	//virtual void integrate(double t);
};

// --- STATIC ---
// para elementos fijos en la escena
class RigidBodyStatic : public RigidBody
{
public:
	RigidBodyStatic(Scene* scn, PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, float size, Vector4 color);

	Vector3 getPosition() const override { return actor->getGlobalPose().p; }

	void setPosition(Vector3 pos = { 0,0,0 }) override
	{
		Entity::setPosition(pos);
		actor->setGlobalPose(*pose);
	}

	void setShape(PxShape* sh, float si) override
	{
		Entity::setShape(sh, si);
		actor->attachShape(*shape);
	}

	void setVisible(bool v = true) override
	{
		Entity::setVisible(v);
		//scene->setActorVisible(actor, v);
	}

	void setRotation(PxQuat rot) override
	{
		Entity::setRotation(rot);
		actor->setGlobalPose(*pose);
	}

private:
	physx::PxRigidStatic* actor = nullptr; // puntero al actor estatico
};

// --- DYNAMIC ---
// para elementos dinamicos en la escena (cambian su posicion)
class RigidBodyDynamic : public RigidBody
{

};