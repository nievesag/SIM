#include "ForceGenerator.h"

ForceGenerator::ForceGenerator(Vector3 pos, float areaR, Scene* s)
    : areaPos(pos), areaRadius(areaR)
{
    // construye area
    const PxTransform* areat = new PxTransform(areaPos);
    area = new RenderItem(CreateShape(PxSphereGeometry(areaRadius)), areat, { 1,0,0,0.5 });
}

ForceGenerator::~ForceGenerator()
{
    // para eliminarlo de la escena:
    DeregisterRenderItem(area);
    delete area;
}

bool ForceGenerator::showArea()
{
    return false;
}

// ------- GENERADOR GRAVEDAD -------
Vector3 GravityGenerator::generateForce(Entity& e)
{
    // si esta dentro del area de actuacion se aplica la fuerza 
    if ((e.getPosition() - areaPos).magnitude() <= areaRadius) 
    {
        return gravity * e.getMass();
    }
    // si esta fuera no se aplica fuerza
}

// ------- GENERADOR VIENTO -------
Vector3 WindGenerator::generateForce(Entity& e)
{
    return Vector3();
}

// ------- GENERADOR TORBELLINO -------
Vector3 WhirlGenerator::generateForce(Entity& e)
{
    return Vector3();
}