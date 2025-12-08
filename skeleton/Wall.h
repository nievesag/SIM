#pragma once
#include "core.hpp"
#include "RigidBody.h"
#include "Scene.h"

class Wall : public RigidBodyStatic
{
public:
	Wall(Scene* scn, float size, Vector3 pos, bool empty, PxPhysics* gPhysics, PxScene* pxScn);

	int getWallSize() const { return wallSize; }
	bool getEmpty() const { return isEmpty; }

private:
	int wallSize = 0;
	bool isEmpty = false;
};
