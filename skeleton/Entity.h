#pragma once
#include <foundation/PxTransform.h>

#include "core.hpp"
#include "RenderUtils.hpp"

class Scene;

class Entity
{
public:
	Entity(Scene* scn);
	virtual ~Entity()  { DeregisterRenderItem(renderItem); };

	virtual void step(double t); // update

	// --- getters
	virtual Vector3 getPosition() const { return pose->p; }			// transform p
	virtual physx::PxQuat getRotation() const { return pose->q; }		// transform q
	double getSize() const { return size; }								// tamano
	double getMass() const { return mass; }								// masa
	Vector3 getVelocity() const { return vel; }						// velocidad
	Vector4 getColor() const { return color; }						// color
	bool getAlive() const { return alive; }								// vivo

	// --- setters
	virtual void setPosition(Vector3 pos) { pose->p = pos; }
	virtual void setRotation(physx::PxQuat rot) { pose->q = rot; }
	virtual void setShape(physx::PxShape* shp, float siz)
	{
		renderItem->shape = shp;
		shape = shp;
		size = siz;
	}
	void setSize(float siz) noexcept { size = siz; }
	void setMass(float mas) noexcept { mass = mas; }
	void setVelocity(Vector3 v) noexcept { vel = v; }
	void setColor(Vector4 col)
	{
		renderItem->color = col;
		color = col;
	}
	void setAlive(bool a) { alive = a; }

protected:
	// --- atributos
	RenderItem* renderItem = nullptr;			// renderItem
	physx::PxShape* shape = nullptr;			// forma
	Vector4 color = { 1,1,1,1 };	// color
	physx::PxTransform* pose = nullptr;			// transform
	double size = 5;							// tamano
	double mass = 1;							// masa
	Vector3 vel = { 0,0,0 };			// velocidad, solo cambia con la aceleracion y la aceleracion cambia mediante una fuerza

	bool alive = true;

	Scene* scene;
};
