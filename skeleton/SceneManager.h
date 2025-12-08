#pragma once

#include "Scene.h"

#include <vector>
#include <PxPhysics.h>
using namespace std;
using namespace physx;

class SceneManager
{
private:
	std::vector<Scene*> vScenes;
	size_t currentScene = 0;

	PxPhysics* gPhysics = nullptr;
	PxScene* gScene = nullptr;

public:
	SceneManager(PxPhysics* px_physics, PxScene* px_scene);
	~SceneManager();

	// gestion escenas
	void addScene(Scene* scn);
	void changeScene(size_t scnId);

	void step(double t); // update
	void keyPressed(unsigned char key, const physx::PxTransform& camera); // input
	void specialKeyPressed(int key, const physx::PxTransform& camera); // input
};

