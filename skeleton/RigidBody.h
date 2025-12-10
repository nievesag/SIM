#pragma once

#include <PxPhysicsAPI.h>
#include "Scene.h"
#include "Entity.h"

enum Shape { BOX, SPHERE, CAPSULE };

class RigidBody : public Entity
{
public:
	RigidBody(Scene* scn) : Entity(scn) {}
	~RigidBody() override = default;

	// set/get mass
	// setMassSpaceInertiaTensor
	// set/getLinearVelocity
	// set/getAngularVelocity
	// addForce()
	// addTorque()
	// Attach Shape

	virtual physx::PxActor* getActor() { return nullptr; }

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
	RigidBodyStatic(Scene* scn, physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 pos, float siz, Vector4 col);

	Vector3 getPosition() const override { return actor->getGlobalPose().p; }

	void setPosition(Vector3 pos = { 0,0,0 }) override
	{
		Entity::setPosition(pos);
		actor->setGlobalPose(*pose);
	}

	void setShape(physx::PxShape* sh, float si) override
	{
		Entity::setShape(sh, si);
		actor->attachShape(*shape);
	}

	void setVisible(bool v = true) override
	{
		Entity::setVisible(v);
		//scene->setActorVisible(actor, v);
	}

	void setRotation(physx::PxQuat rot) override
	{
		Entity::setRotation(rot);
		actor->setGlobalPose(*pose);
	}

	physx::PxRigidActor* getActor() override { return actor; }

private:
	physx::PxRigidStatic* actor = nullptr; // puntero al actor estatico
};

// --- DYNAMIC ---
// para elementos dinamicos en la escena (cambian su posicion)
class RigidBodyDynamic : public RigidBody
{
public:
	// scn, gphys, gscn, mat, kin,pos, vel, size, volume, color, mass, damp, maxLT, shape, density, angVel, tensor
	RigidBodyDynamic(Scene* scn = nullptr,
	                 physx::PxPhysics* gPhysics = nullptr, physx::PxScene* gScene = nullptr, physx::PxMaterial* mat = nullptr, bool kin = false,
		Vector3 pos = { 0,0,0 }, Vector3 vel = { 0,0,0 }, float siz = 5, physx::PxVec3 vol = { 1,1,1 },
		Vector4 col = { 1,1,1,1 }, float m = 10, float damp = 0.8, float maxLT = -1,
		Shape sh = SPHERE, double density = -1,
	                 physx::PxVec3 angVel = { 0, 0, 0 }, physx::PxVec3 tensor = { 1,1,1 });
	~RigidBodyDynamic() override = default;

	void step(double t) override;

	// fuerzas
	void addForce(float x, float y, float z);
	void addForce(const Vector3& fc);
	void applyForce();

	virtual bool collisionCallback() { return false; }

	// -- setters
	void setLinearVelocity(physx::PxVec3 vel = { 0, 0, 0 }) // velocidades
	{
		RigidBody::setVelocity(vel);
		actor->setLinearVelocity(vel);
	}

	void setAngularVelocity(physx::PxVec3 vel = { 0, 0, 0 })
	{
		actor->setAngularVelocity(vel);
	}

	void setPosition(physx::PxVec3 pos = { 0,0,0 }) override // posicion
	{
		RigidBody::setPosition(pos);
		actor->setGlobalPose(*pose);
	}

	void setShape(physx::PxShape* shp, float siz = 1) override // shape
	{
		Entity::setShape(shp, siz);
		actor->attachShape(*getShape());
	}

	void setVisible(bool vis = true) override // visible
	{
		Entity::setVisible(vis);
		//scene->setActorVisible(actor, vis);
	}

	void setDensity(float d) // density
	{
		density = d;
	}

	void setRotation(double rot, physx::PxVec3 axis = { 0, 0, 1 }) // rotation
	{
		angle += rot;

		physx::PxTransform c = actor->getGlobalPose();
		physx::PxQuat q(angle, axis);
		physx::PxTransform newPose(c.p, q);

		actor->setGlobalPose(newPose);
		actor->setKinematicTarget(newPose);
	}

	void setMass(float mas) override // mass
	{
		actor->setMass(mas);
		float cojones = actor->getMass();
		Entity::setMass(mas);
	}

	void setKinematic(bool k) // kinematic 
	{
		// Each simulation step PhysX moves the actor to its target position, regardless of external forces, gravity, collision, etc
		actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, k);
	}

	//void setGroup(PxU32 group, bool autoexcluding = true);

	void setActorFlag(physx::PxActorFlag::Enum flag, bool value) // actor flag
	{
		actor->setActorFlag(flag, value);
	}

	void setDamping(float damping) // damping
	{
		actor->setLinearDamping(damping);
		actor->setAngularDamping(damping);
	}

	// getters
	physx::PxRigidActor* getActor() override { return actor; }
	Vector3 getPosition() const override { return actor->getGlobalPose().p; }
	physx::PxQuat getRotation() const override { return actor->getGlobalPose().q; }
	Vector3 getVelocity() const override { return actor->getLinearVelocity(); }
	double getMass() const override { return actor->getMass(); }
	//RigidBodyGenerator* getGenerator() const { return generator; }

	//updateMassAndInertia

private:
	physx::PxRigidDynamic* actor = nullptr; // puntero al actor dinamico

	physx::PxScene* gScene = nullptr;
	physx::PxMaterial* gMaterial = nullptr;
	double density = 1;
	double angle = 0;

	Shape sh;

	std::vector<Vector3> resultingForce; // fuerza resultante

	// --- atributos
	// - fisicos
	Vector3 gravity = { 0, -9.8f, 0 }; // gravedad
	Vector3 acc = { 0,0,0 }; // aceleracion
	float damping = 0.99f; // damping -> v=v*d^t

	// - gestion
	float lifetime;
	float maxLifetime = -1;

	//virtual void integrate(double t);

	void manageLife(double t);

	//RigidBodyGenerator* generator = nullptr;
};