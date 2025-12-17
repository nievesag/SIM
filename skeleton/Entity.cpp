#include "Entity.h"

Entity::Entity(Scene* scn)
	: scene(scn)
{
}

void Entity::step(double t)
{
	toggleVisibility();
}