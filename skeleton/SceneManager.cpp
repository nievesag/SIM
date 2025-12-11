#include "SceneManager.h"

#include <cctype>
#include <iostream>

SceneManager::SceneManager(PxPhysics* px_physics, PxScene* px_scene) :
	gPhysics(px_physics), gScene(px_scene)
{
	//addScene(new Scene0(gPhysics, gScene));
	//addScene(new Scene1());
	//addScene(new Scene2());
	//addScene(new Scene3());
	//addScene(new Scene4());
	addScene(new Scene5(gPhysics, gScene));
	//addScene(new Scene6(gPhysics, gScene));
}

SceneManager::~SceneManager()
{
	for (auto* s : vScenes) delete s;
}

void SceneManager::addScene(Scene* scn)
{
	if (scn != nullptr) 
	{
		vScenes.push_back(scn);
		scn->init();
	}
}

void SceneManager::changeScene(size_t scnId)
{
	// Solo si la escena existe
	if (scnId < vScenes.size())
	{
		// Cambiar si es una nueva escena
		if (scnId != currentScene)
		{
			vScenes[currentScene]->unload(); // quitar escena anterior
			currentScene = scnId;
			vScenes[currentScene]->load(); // poner escena nueva
			cout << "[SCENE] Escena " << std::to_string(scnId) << ".\n";
		}
	}
	else
	{
		cout << "[NOTA] No existe la escena " << std::to_string(scnId) << ".\n";
	}
}

void SceneManager::step(double t)
{
	vScenes[currentScene]->step(t);
}

void SceneManager::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	PX_UNUSED(camera);

	if (key >= '0' && key <= '9')
	{
		changeScene(key - '0');
	}
	else
	{
		for (auto e : vScenes) e->keyPressed(key, camera);
	}

	/*
	switch (toupper(key))
	{
	case '0':
		changeScene(0);
		cout << "[SCENE] Escena 1" << ".\n";
		break;
	case'1':
		cout << "[SCENE] Escena 2" << ".\n";
		changeScene(1);
		break;

	default:
		for (auto e : vScenes) e->keyPressed(key, camera);
		break;
	}
	*/
}

void SceneManager::specialKeyPressed(int key, const physx::PxTransform& camera)
{
	PX_UNUSED(camera);

	for (auto e : vScenes) e->specialKeyPressed(key, camera);
}