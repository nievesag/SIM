#pragma once
#include "RigidBody.h"
//#include "core.hpp"
//#include "Scene.h"
//class RigidBodyStatic;

class Magnet : public RigidBodyStatic
{
public:
	Magnet(Scene* scn, float size, Vector3 pos, Vector4 col, physx::PxPhysics* gPhysics, physx::PxScene* pxScn);
};