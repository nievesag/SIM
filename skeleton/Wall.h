#pragma once
#include "core.hpp"
#include "Scene.h"

class Wall : public Entity
{
public:
	Wall(Scene* scn, int size, Vector3 pos, bool empty);

	int getWallSize() const { return wallSize; }
	bool getEmpty() const { return isEmpty; }

private:
	int wallSize = 0;
	bool isEmpty = false;
};
