#pragma once
#include "RigidBody.h"
//#include "core.hpp"
//#include "Scene.h"
//class RigidBodyStatic;

class Magnet : public RigidBodyDynamic
{
public:
	Magnet(Scene* scn, float size, Vector3 pos, Vector4 col, physx::PxPhysics* gPhysics, physx::PxScene* pxScn, float magneticFieldR);
	~Magnet();
	void updateMagneticField(Vector3 pos);

private:
	Entity* magneticField = nullptr;
	//RenderItem* magneticField = nullptr;
};