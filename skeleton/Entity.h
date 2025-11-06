#pragma once
#include <foundation/PxTransform.h>

#include "core.hpp"
#include "RenderUtils.hpp"

class Scene;

class Entity
{
public:
	Entity(Scene* scn);
	virtual ~Entity()  { DeregisterRenderItem(renderItem); }

	virtual void step(double t); // update

	// --- getters
	virtual Vector3 getPosition() const { return pose->p; }			// transform p
	virtual physx::PxQuat getRotation() const { return pose->q; }		// transform q
	double getSize() const { return size; }								// tamano
	double getMass() const { return mass; }								// masa
	Vector3 getVelocity() const { return velocity; }						// velocidad
	Vector4 getColor() const { return color; }						// color
	bool getAlive() const { return alive; }								// vivo
	RenderItem* getRenderItem() const { return renderItem; }

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
	void setVelocity(Vector3 v) noexcept { velocity = v; }
	void setColor(Vector4 col)
	{
		renderItem->color = col;
		color = col;
	}
	void setAlive(bool a) { alive = a; }
	void setVisible(bool v)
	{
		visible = v;
	}
	void toggleVisibility()
	{
		if (visible) RegisterRenderItem(renderItem);
		else DeregisterRenderItem(renderItem);
	}

protected:
	// --- atributos
	physx::PxTransform* pose = nullptr;			// transform
	double size = 5;							// tamano
	physx::PxShape* shape = nullptr;			// forma
	Vector4 color = { 1,1,1,1 };	// color
	RenderItem* renderItem = nullptr;			// renderItem
	double mass = 1;							// masa
	Vector3 velocity = { 0,0,0 };			// velocidad, solo cambia con la aceleracion y la aceleracion cambia mediante una fuerza

	bool alive = true;
	bool visible = true;

	Scene* scene = nullptr;
};
