#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(Vector3 pos, Scene* s)
    : areaPos(pos)
{
    const PxTransform* t = new PxTransform({ pos.x, pos.y, pos.z });
    area = new RenderItem(CreateShape(PxSphereGeometry(areaRadius)), t, { 1,1,1,0.5 });
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
    // si esta fuera se aplica fuerza 0
}
