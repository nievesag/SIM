#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(Vector3 pos, Scene* s)
    : areaPos(pos)
{

}

ForceGenerator::~ForceGenerator()
{

}

Vector3 GravityGenerator::generateForce(Entity& e)
{
    // si esta dentro del area de actuacion se aplica la fuerza 
    if ((e.getPosition() - areaPos).magnitude() <= areaRadius) 
    {
        return gravity * e.getMass();
    }
    // si esta fuera no se aplica fuerza
}
