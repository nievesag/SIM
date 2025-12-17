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

	// ---------- getters
	virtual Vector3 getPosition() const { return pose->p; }			// transform p
	virtual physx::PxQuat getRotation() const { return pose->q; }		// transform q
	virtual double getSize() const { return size; }						// tamano
	virtual double getMass() const { return mass; }						// masa
	virtual Vector3 getVelocity() const { return velocity; }			// velocidad
	virtual Vector4 getColor() const { return color; }				// color
	virtual bool getAlive() const { return alive; }						// vivo
	virtual RenderItem* getRenderItem() const { return renderItem; }	// renderitem
	virtual float getq() const { return q; }							// q (carga)
	virtual Scene* getScene() const { return scene; }					// scene
	virtual physx::PxShape* getShape() const { return shape; }			// shape

	// ---------- setters
	virtual void setPosition(Vector3 pos) { pose->p = pos; }			// position
	virtual void setRotation(physx::PxQuat rot) { pose->q = rot; }		// rotation
	virtual void setShapeWithSize(physx::PxShape* shp, float siz)		// shape w/ size
	{
		renderItem->shape = shp;
		shape = shp;
		size = siz;
	}
	virtual void setShape(physx::PxShape* sh, float si)					// shape
	{
		renderItem->shape = sh;
		shape = sh;
		size = si;
	}
	virtual void setSize(float siz) { size = siz; }						// size
	virtual void setMass(float mas)										// mass
	{
		mass = mas;
	}
	virtual void setVelocity(Vector3 v) { velocity = v; }				// velocity
	virtual void setColor(Vector4 col)									// color
	{
		renderItem->color = col;
		color = col;
	}
	virtual void setAlive(bool a) { alive = a; }						// color
	virtual void setVisible(bool v)										// visible
	{
		visible = v;
	}
	virtual void toggleVisibility()										// toggle visibility
	{
		if (visible) RegisterRenderItem(renderItem);
		else DeregisterRenderItem(renderItem);
	}
	virtual void setq(float newq) { q = newq; }							// q
	virtual void setRenderItem(RenderItem* ri) { renderItem = ri; }		// renderitem
	virtual void setPose(physx::PxTransform* p) { pose = p; }			// pose

protected:
	// --- atributos
	physx::PxTransform* pose = nullptr;			// transform
	float size = 5;								// tamano
	physx::PxShape* shape = nullptr;			// forma
	Vector4 color = { 1,1,1,1 };	// color
	RenderItem* renderItem = nullptr;			// renderItem
	float mass = 1;								// masa
	Vector3 velocity = { 0,0,0 };		// velocidad, solo cambia con la aceleracion y la aceleracion cambia mediante una fuerza

	bool alive = true;
	bool visible = true;

	Scene* scene = nullptr;

	float q = 0; // carga magnetica de la particula
};