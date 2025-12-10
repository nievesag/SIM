#pragma once
#include "core.hpp"
#include "RigidBody.h"
#include "Scene.h"

class Magnet : public RigidBodyStatic
{
public:
	Magnet(Scene* scn, float size, Vector3 pos, Vector4 col, PxPhysics* gPhysics, PxScene* pxScn);
};