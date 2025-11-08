#include "Pipe.h"

#include "Scene.h"

Pipe::Pipe(Scene* scn, Vector3 pos)
	: Entity(scn)
{
	orgPos = pos;
	maxPos = { pos.x, pos.y - maxYpos, pos.z };
	pose = new physx::PxTransform(pos);
	shape = CreateShape(physx::PxBoxGeometry(10, pipeLenght, 10));
	color = { 0.4,0.5,0.6,1 };
	renderItem = new RenderItem(shape, pose, color);
	RegisterRenderItem(renderItem);
}

void Pipe::step(double t)
{
	if (state != -1)
	{
		if (pose->p.y > maxPos.y)
		{
			if (state == 0)
			{
				pose->p += vel * t;
			}
			if (state == 1)
			{
				if (pose->p.y < orgPos.y)
				{
					pose->p -= vel * t;
				}
			}
		}
		else
		{
			std::cout << "bala" << std::endl;
			scene->newToy({ pose->p.x, pose->p.y - (pipeLenght / 2) , pose->p.z });
			state = 1;
			pose->p -= vel * t;
		}
	}
}