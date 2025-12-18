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

Vector3 HorizontalWhirlGenerator::generateForce(Entity& e)
{
    if (inArea(e) && isActive)
    {
        return k * Vector3(-(e.getPosition().z - areaPos.z), 50 - (e.getPosition().y - areaPos.y), e.getPosition().x - areaPos.x);
    }
    return { 0,0,0 };
}

// ------- GENERADOR MAGNETISMO -------
MagnetismGenerator::MagnetismGenerator(Vector3 pos, float areaR, Scene* scn, float B, PxPhysics* gPhysics, PxScene* pxScn)
    : ForceGenerator(pos, areaR, scn, false), b(B), s(pxScn)
{
    // construye el iman
    magnetPose = new PxTransform(pos);

    Vector4 color = { 1,1,1,1 };
    if (b < 0) color = { 1,0,0,1 }; // norte
    else if (b > 1) color = { 0,0,1,1 }; // sur

    magnet = new Magnet(scn, 10, pos, color, gPhysics, pxScn, areaR);
}

Vector3 MagnetismGenerator::generateForce(Entity& e)
{
    if (inArea(e) && e.getq() != 0 && isActive) // si esta en el area y es una particula cargada
    {
        // F = (u q1 * q2) / (4 pi r^2)

        double u = 0.04 * M_PI; // Permeabilidad del medio, vacio
    	double F = u * (b * e.getq())/ 4 * M_PI * std::pow((e.getPosition() - areaPose->p).magnitude(), 2);

        Vector3 action = (e.getPosition() - areaPose->p);

        return F * action;
    }
    return { 0,0,0 };
}

void MagnetismGenerator::move(Vector3 dir)
{
    areaPose->p += dir * 5;
    magnetPose->p += dir * 5;
    areaPos = areaPose->p;
    magnet->setPosition(magnet->getPosition() += dir * 5);
    magnet->updateMagneticField(magnet->getPosition());
}

void MagnetismGenerator::setPos(Vector3 newPos)
{
    areaPose->p = newPos;
    magnetPose->p = newPos;
}

void MagnetismGenerator::toggleForce()
{
    ForceGenerator::toggleForce();
    
    isActive ? magnet->showField() : magnet->hideField();
}

void MagnetismGenerator::toggleMagnetVisibility()
{
    if (areaVisible) RegisterRenderItem(magnet->getRenderItem());
    else 
    {
        magnet->hideField();
        DeregisterRenderItem(magnet->getRenderItem());
        s->removeActor(*magnet->getActor());
    }
}

void MagnetismGenerator::toggleAreaVisibility() {
    //ForceGenerator::toggleAreaVisibility();
}

// ------- GENERADOR MUELLES -------
SpringForceGenerator::SpringForceGenerator(double _k, double _restingLength, Entity* _other)
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
BuoyancyForceGenerator::BuoyancyForceGenerator(Vector3 pos, float areaR, Vector3 liquidXZ, Scene* s, float h, float V, float d)
    : ForceGenerator(pos, areaR, s, false), height(h), volume(V), liquidDensity(d), liquidSize(liquidXZ)
{
   fluid = new RenderItem(CreateShape(PxBoxGeometry(liquidXZ.x,height,liquidXZ.z)), new physx::PxTransform(pos), { 0,0,1,0.3f });
}

Vector3 BuoyancyForceGenerator::generateForce(Entity& e)
{
    Vector3 force = { 0,0,0 };

    // esta en la zona del fluido
    if (e.getPosition().x <= areaPos.x + liquidSize.x && 
        e.getPosition().x >= areaPos.x - liquidSize.x &&
        e.getPosition().z <= areaPos.z + liquidSize.z &&
        e.getPosition().z >= areaPos.z - liquidSize.z)
    {
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
    }
    
    return force;
}

// ------- GENERADOR EXPLOSION -------
ExplosionForceGenerator::ExplosionForceGenerator(Vector3 pos, float areaR, Scene* s, bool visibleArea, float _k, float _t)
	: k(_k), t(_t), ForceGenerator(pos, areaR, s, visibleArea)
{
    isActive = false;
}

Vector3 ExplosionForceGenerator::generateForce(Entity& e)
{
    Vector3 force = { 0,0,0 };

    if (isActive)
    {
        float lf = static_cast<Particle&>(e).getLifetime();

        if (lf >= duration)
        {
            toggleForce();
        }

        // distancia a la explosion
        float distance = (e.getPosition() - areaPos).magnitude();
        Vector3 r = e.getPosition() - areaPos;

        if (inArea(e))
        {
            force = (k / pow(distance, 2)) * r * (exp(-lf / t));
        }
    }

    return force;
}
