#pragma once
#include <foundation/PxTransform.h>

#include "core.hpp"
#include "RenderUtils.hpp"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void step(double t); // update

	// --- getters
	virtual Vector3 getPosition() const { return pose->p; }			// transform
	virtual physx::PxQuat getRotation() const { return pose->q; }		// transform
	double getSize() const { return size; }								// tamano
	double getMass() const { return mass; }								// masa
	Vector3 getVelocity() const { return vel; }						// velocidad

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

private:
	// --- atributos
	RenderItem* renderItem = nullptr;			// renderItem
	physx::PxTransform* pose = nullptr;			// transform
	physx::PxShape* shape = nullptr;			// forma
	double size = 5;							// tamano
	double mass = 1;							// masa
	Vector3 vel = { 0,0,0 };			// velocidad
	Vector4 color = { 1,1,1,1 };	// color
};
