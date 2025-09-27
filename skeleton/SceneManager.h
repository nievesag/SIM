#pragma once

#include "Scene.h"

#include <vector>
#include <foundation/PxTransform.h>
using namespace std;

class SceneManager
{
private:
	std::vector<Scene*> vScenes;
	size_t currentScene = 0;

public:
	SceneManager();
	~SceneManager();

	// gestion escenas
	void addScene(Scene* scn);
	void changeScene(size_t scnId);

	void step(double t); // update
	void keyPressed(unsigned char key, const physx::PxTransform& camera); // input
};

