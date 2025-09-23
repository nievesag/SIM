#pragma once

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Vector3D.h"

#include <iostream>

using namespace physx;

class axis
{
public:
	axis() 
	{
		tO = new PxTransform({ 0, 0, 0 });
		bolaO = new RenderItem(CreateShape(PxSphereGeometry(1)), tO, { 1,1,1,1 });

		tX = new PxTransform({ separacion * ax{}.X.getX(), 0, 0 });
		bolaX = new RenderItem(CreateShape(PxSphereGeometry(1)), tX, { 1,0,0,1 });

		tY = new PxTransform({ 0, separacion * ax{}.Y.getY(), 0 });
		bolaY = new RenderItem(CreateShape(PxSphereGeometry(1)), tY, { 0,1,0,1 });

		tZ = new PxTransform({ 0, 0 , separacion * ax{}.Z.getZ() });
		bolaZ = new RenderItem(CreateShape(PxSphereGeometry(1)), tZ, { 0,0,1,1 });
	}

	~axis()
	{
		DeregisterRenderItem(bolaO);
		delete bolaO;

		DeregisterRenderItem(bolaX);
		delete bolaX;

		DeregisterRenderItem(bolaY);
		delete bolaY;

		DeregisterRenderItem(bolaZ);
		delete bolaZ;

		delete tO;
		delete tX;
		delete tY;
		delete tZ;
	}

private:
	struct ax
	{
		const Vector3D X = { 1.0,0.0,0.0 };
		const Vector3D Y = { 0.0,1.0,0.0 };
		const Vector3D Z = { 0.0,0.0,1.0 };
		const Vector3D O = { 0.0,0.0,0.0 };
	};

	float separacion = 10;

	const PxTransform* tO = nullptr;
	RenderItem* bolaO = nullptr;

	const PxTransform* tX = nullptr;
	RenderItem* bolaX = nullptr;

	const PxTransform* tY = nullptr;
	RenderItem* bolaY = nullptr;

	const PxTransform* tZ = nullptr;
	RenderItem* bolaZ = nullptr;
};

