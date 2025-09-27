#include "Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{
	DeregisterRenderItem(renderItem);
}

void Entity::step(double t)
{

}
