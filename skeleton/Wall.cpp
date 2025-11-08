#include "Wall.h"

Wall::Wall(Scene* scn, int size, Vector3 pos, bool empty)
	: Entity(scn), wallSize(size), isEmpty(empty)
{
	pose = new physx::PxTransform(pos);
	shape = CreateShape(physx::PxBoxGeometry(size, size, size));
	if (!isEmpty)
	{
		color = { 0.5,0.8,0.03,1 };
	}
	else
	{
		color = { 1,0,0,0.5};
	}
	renderItem = new RenderItem(shape, pose, color);
}