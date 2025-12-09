#pragma once

#include <PxPhysicsAPI.h>
#include "Scene.h"
#include "Entity.h"

enum Shape { BOX, SPHERE, CAPSULE };

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
	RigidBodyStatic(Scene* scn, PxPhysics* gPhysics, PxScene* gScene, Vector3 pos, float siz, Vector4 col);

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
public:
	RigidBodyDynamic(Scene* scn = nullptr, 
		PxPhysics* gPhysics = nullptr, PxScene* gScene = nullptr, PxMaterial* mat = nullptr, bool kin = false,
		Vector3 pos = { 0,0,0 }, Vector3 vel = { 0,0,0 }, float siz = 5, PxVec3 vol = { 1,1,1 },
		Vector4 col = { 1,1,1,1 }, float m = 10, float damp = 0.8, float maxLT = -1,
		Shape sh = SPHERE, double density = -1,
		PxVec3 angVel = { 0, 0, 0 }, PxVec3 tensor = { 1,1,1 });

	void step(double t) override;

	// fuerzas
	void addForce(float x, float y, float z);
	void addForce(const Vector3& fc);
	void applyForce();

	virtual bool collisionCallback() { return false; }

	// -- setters
	void setLinearVelocity(PxVec3 vel = { 0, 0, 0 }) // velocidades
	{
		RigidBody::setVelocity(vel);
		actor->setLinearVelocity(vel);
	}

	void setAngularVelocity(PxVec3 vel = { 0, 0, 0 })
	{
		actor->setAngularVelocity(vel);
	}

	void setPosition(PxVec3 pos = { 0,0,0 }) override // posicion
	{
		RigidBody::setPosition(pos);
		actor->setGlobalPose(*pose);
	}

	void setShape(PxShape* shp, float siz = 1) override // shape
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

	void setRotation(double rot, PxVec3 axis = { 0, 0, 1 }) // rotation
	{
		angle += rot;

		PxTransform c = actor->getGlobalPose();
		PxQuat q(angle, axis);
		PxTransform newPose(c.p, q);

		actor->setGlobalPose(newPose);
		actor->setKinematicTarget(newPose);
	}

	void setMass(float mas) override // mass
	{
		actor->setMass(mas);
		Entity::setMass(mas);
	}

	void setKinematic(bool k) // kinematic 
	{
		// Each simulation step PhysX moves the actor to its target position, regardless of external forces, gravity, collision, etc
		actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, k);
	}

	//void setGroup(PxU32 group, bool autoexcluding = true);

	void setActorFlag(PxActorFlag::Enum flag, bool value) // actor flag
	{
		actor->setActorFlag(flag, value);
	}

	void setDamping(float damping) // damping
	{
		actor->setLinearDamping(damping);
		actor->setAngularDamping(damping);
	}

	// getters
	PxRigidActor* getActor() override { return actor; }
	Vector3 getPosition() const override { return actor->getGlobalPose().p; }
	PxQuat getRotation() const override { return actor->getGlobalPose().q; }
	Vector3 getVelocity() const override { return actor->getLinearVelocity(); }
	double getMass() const override { return actor->getMass(); }
	//RigidBodyGenerator* getGenerator() const { return generator; }

	//updateMassAndInertia

private:
	physx::PxRigidDynamic* actor = nullptr; // puntero al actor dinamico

	PxScene* gScene = nullptr;
	PxMaterial* gMaterial = nullptr;
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