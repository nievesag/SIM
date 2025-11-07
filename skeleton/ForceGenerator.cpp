#include "ForceGenerator.h"

#define _USE_MATH_DEFINES
#include <cmath> 

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

bool ForceGenerator::inArea(Entity& e)
{
    return ((e.getPosition() - areaPos).magnitude() <= areaRadius);
}

// ------- GENERADOR GRAVEDAD -------
Vector3 GravityGenerator::generateForce(Entity& e)
{
    // si esta dentro del area de actuacion se aplica la fuerza 
    if (inArea(e)) 
    {
        return gravity * e.getMass();
    }
    // si esta fuera no se aplica fuerza
    return { 0,0,0 };
}

// ------- GENERADOR VIENTO -------
Vector3 WindGenerator::generateForce(Entity& e)
{
    if (inArea(e))
    {
        return (k1 * (wind - e.getVelocity()) + k2);
    }
    return { 0,0,0 };
}

// ------- GENERADOR TORBELLINO -------
Vector3 WhirlGenerator::generateForce(Entity& e)
{
    if (inArea(e))
    {
        return k * Vector3(-(e.getPosition().z - areaPos.z), 50 - (e.getPosition().y - areaPos.y), e.getPosition().x-areaPos.x);
    }
    return { 0,0,0 };
}

MagnetismGenerator::MagnetismGenerator(Vector3 pos, float areaR, Scene* scn, int p, float B)
    : ForceGenerator(pos, areaR, scn), pole(p), b(B)
{
    // construye el iman
    physx::PxTransform* pose = new PxTransform(pos);
    physx::PxShape* shape = CreateShape(PxBoxGeometry(10, 10, 10));
    Vector4 color = { 1,1,1,1 };
    if (pole == 0) color = { 1,0,0,1 }; // norte
    else if (pole == 1) color = { 0,0,1,1 }; // sur
    RenderItem* renderItem = new RenderItem(shape, pose, color);
}

Vector3 MagnetismGenerator::generateForce(Entity& e)
{
    if (inArea(e))
    {
        // F=q*(vxB)
        //return e.getq() * (e.getVelocity().cross(B));
        Vector3 force = 
            (e.getq() * b) / std::pow((e.getPosition() - areaPos).magnitude()
        return (1 / 4 * std::atan(1.0)) * ( / );
    }
    return { 0,0,0 };
}
