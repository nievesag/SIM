#pragma once
#include "Entity.h"

class Pipe : public Entity
{
public:
	Pipe(Scene* scn, Vector3 pos);

	void step(double t) override;
	void eject() { state = 0; }

private:
	float maxYpos = 20;
	physx::PxVec3 maxPos;
	physx::PxVec3 orgPos;
	Vector3 vel = {0, -9.8, 0};
	float pipeLenght = 30;

	int state = -1; // 0 = bajar / 1 = subir
};