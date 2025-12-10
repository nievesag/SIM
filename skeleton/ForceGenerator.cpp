#include "ForceGenerator.h"

#define _USE_MATH_DEFINES
#include <math.h> 
#include "Magnet.h"

ForceGenerator::ForceGenerator(Vector3 pos, float areaR, Scene* s, bool visibleArea)
    : areaPos(pos), areaRadius(areaR)
{
	areaPose = new PxTransform(areaPos);

    if (visibleArea)
    {
	    // construye area
	    area = new RenderItem(CreateShape(PxSphereGeometry(areaRadius)), areaPose, { 1,1,1,0.18f });
    }
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
    if (inArea(e) && isActive) 
    {
        return gravity * e.getMass();
    }
    // si esta fuera no se aplica fuerza
    return { 0,0,0 };
}

// ------- GENERADOR VIENTO -------
Vector3 WindGenerator::generateForce(Entity& e)
{
    if (inArea(e) && isActive)
    {
        return (k1 * (wind - e.getVelocity()) + k2);
    }
    return { 0,0,0 };
}

// ------- GENERADOR TORBELLINO -------
Vector3 WhirlGenerator::generateForce(Entity& e)
{
    if (inArea(e) && isActive)
    {
        return k * Vector3(-(e.getPosition().z - areaPos.z), 50 - (e.getPosition().y - areaPos.y), e.getPosition().x-areaPos.x);
    }
    return { 0,0,0 };
}

// ------- GENERADOR MAGNETISMO -------
MagnetismGenerator::MagnetismGenerator(Vector3 pos, float areaR, Scene* scn, float B, PxPhysics* gPhysics, PxScene* pxScn)
    : ForceGenerator(pos, areaR, scn, false), b(B)
{
    // construye el iman
    magnetPose = new PxTransform(pos);
    physx::PxShape* shape = CreateShape(PxBoxGeometry(10, 10, 10));

    Vector4 color = { 1,1,1,1 };
    if (b < 0) color = { 1,0,0,1 }; // norte
    else if (b > 1) color = { 0,0,1,1 }; // sur

    magnet = new Magnet(scn, 10, pos, color, gPhysics, pxScn);

    //magnet = new RenderItem(shape, magnetPose, color);
    //RegisterRenderItem(magnet);
}

Vector3 MagnetismGenerator::generateForce(Entity& e)
{
    if (inArea(e) && e.getq() != 0 && isActive) // si esta en el area y es una particula cargada
    {
        // F = (u q1 * q2) / (4 pi r^2)

        // falta permeabilidad del medio!!!!!!!!!!!
        double u = 0.7; // 4 * pi * 10^-7
    	double F = b * e.getq() / 4 * M_PI * std::pow((e.getPosition() - areaPose->p).magnitude(), 2);

        Vector3 action = (e.getPosition() - areaPose->p);

        return F * action;
    }
    return { 0,0,0 };
}

void MagnetismGenerator::move(Vector3 dir)
{
    areaPose->p += dir * 5;
    magnetPose->p += dir * 5;
}

void MagnetismGenerator::setPos(Vector3 newPos)
{
    areaPose->p = newPos;
    magnetPose->p = newPos;
}

void MagnetismGenerator::toggleMagnetVisibility()
{
    if (areaVisible) RegisterRenderItem(magnet->getRenderItem());
    else DeregisterRenderItem(magnet->getRenderItem());
}

// ------- GENERADOR MUELLES -------
SpringForceGenerator::SpringForceGenerator(double _k, double _restingLength, Particle* _other)
    : k(_k), restingLength(_restingLength), other(_other)
{

}

Vector3 SpringForceGenerator::generateForce(Entity& e)
{
    Vector3 force = { 0,0,0 };

    if (other != nullptr && !other->getAlive())
    {
        other = nullptr;
    }

    if (&e != nullptr && other != nullptr) 
    {
        Vector3 springLenght = other->getPosition() - e.getPosition();
        float realLenght = springLenght.normalize();
        float delta = realLenght - restingLength;

        force = springLenght * delta * k;
    }

    return force;
}

// ------- GENERADOR FLOTACION -------
BuoyancyForceGenerator::BuoyancyForceGenerator(Vector3 pos, float areaR, Scene* s, float h, float V, float d)
    : ForceGenerator(pos, areaR, s, false), height(h), volume(V), liquidDensity(d)
{

}

Vector3 BuoyancyForceGenerator::generateForce(Entity& e)
{
    Vector3 force = { 0,0,0 };

    float h = e.getPosition().y; // centro del objeto que flota
    float h0 = areaPos.y; // centro de la superficie liquida

    float immersed;

    if (h - h0 > height * 0.5f)
    {
        immersed = 0.0;
    }
    else if (h0 - h > height * 0.5f)
    {
        immersed = 1.0;
    }
    else
    {
        immersed = (h0 - h) / height + 0.5f; // profundidad normalizada
    }

    force.y = liquidDensity * volume * immersed * 9.8f;

    return force;
}
