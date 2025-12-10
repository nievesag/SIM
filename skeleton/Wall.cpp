#include "Wall.h"

Wall::Wall(Scene* scn, float size, Vector3 pos, bool empty, PxPhysics* gPhysics, PxScene* pxScn)
	: RigidBodyStatic(scn, gPhysics, pxScn, pos, size, { 0.5,0.8,0.03,1 }), 
		wallSize(size), isEmpty(empty)
{
	if (isEmpty) 
	{
		pxScn->removeActor(*this->getActor());
		DeregisterRenderItem(renderItem);
	}
}