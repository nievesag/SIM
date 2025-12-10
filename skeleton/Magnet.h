#pragma once
#include "core.hpp"
#include "RigidBody.h"

class Magnet : public RigidBodyStatic
{
public:
	Magnet(Scene* scn, float size, Vector3 pos, Vector4 col, PxPhysics* gPhysics, PxScene* pxScn);


private:
	int magnetSize = 0;
	int bCharge = 0;
};