#pragma once
#include <vector>
#include <foundation/PxTransform.h>

#include "Entity.h"
#include "Particle.h"
#include "Projectile.h"
#include "ForceGenerator.h"
#include "ParticleGenerator.h"

#include "Pipe.h"
#include "ProjectileGenerator.h"
#include "RigidBodyGenerator.h"
class RigidBodySystem;
class Wall;

class System;
class ParticleSystem;
class ChargedEntity;
class ChargedRbGenerator;
class MagnetismGenerator;

class Scene
{
public:
	//Scene() = default;
	Scene(PxPhysics* gPhysics = nullptr, PxScene* gScene = nullptr);
	virtual ~Scene();

	virtual void init();

	virtual void step(double t); // update

	void deleteEntities();

	// cambio de escenas
	virtual void load();
	virtual void unload();

	// gestion entidades
	void addEntity(Entity* ent);
	void addSystem(System* sys);

	// input
	virtual void keyPressed(unsigned char key, const physx::PxTransform& camera);
	virtual void specialKeyPressed(int key, const physx::PxTransform& camera); // special keypress event

	// juego
	Vector3 getActionThreshold() { return actionThreshold; }

	virtual void newToy(Vector3 pos) = 0;
	virtual void readFile(std::string file) = 0;
	virtual void splash(Vector3 pos) = 0;

	// px
	PxPhysics* getPxPhysics() const	{ return gPhysics; }
	PxScene* getPxScene() const { return gScene; }

protected:
	std::vector<Entity*> gObjects;		// Entidades de la escena
	std::vector<System*> pSystems; // sistemas de particulas

	Vector3 actionThreshold = {200, 200,200}; // limites de la escena a partir de los cuales se eliminan las particulas
	
	PxPhysics* gPhysics = nullptr;
	PxScene* gScene = nullptr;
};

// --- ESCENAS HIJAS ---
// Scene0, Scene1, Scene2 ...

class Scene0 : public Scene
{
public:
	Scene0() = default;
	Scene0(PxPhysics* gphys, PxScene* gscn) : Scene(gphys, gscn) {}
	void init() override;
	void step(double t) override;
	void load() override;
	void unload() override;
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override;
	void specialKeyPressed(int key, const physx::PxTransform& camera) override;

	void newToy(Vector3 pos) override;

	void readFile(std::string file) override;
	void splash(Vector3 pos) override;

private:
	int width = 0, height = 0; // tamaño del mapa
	std::vector<std::vector<Wall*>> map;

	ParticleSystem* sys = nullptr; // sistema de particulas
	RigidBodySystem* sysRb = nullptr;

	std::vector<MagnetismGenerator*> magnets; // vector de todos los imanes
	MagnetismGenerator* magnetism1 = nullptr; // iman 1
	MagnetismGenerator* magnetism2 = nullptr; // iman 2

	MagnetismGenerator* selectedMagnet = nullptr;	// iman que tienes seleccionado

	ChargedRbGenerator* chargedGenerator = nullptr;	// generador de particulas con carga
	TrailGenerator* trailGenerator = nullptr;		// rastro que deja la particula
	bool fatherPart = false;

	Pipe* pipe = nullptr; // tuberia que suelta la particula

	ForceGenerator* viento = nullptr;
	ForceGenerator* torbellino = nullptr;
	ParticleGenerator* randomMass = nullptr;

	ProjectileGenerator* pGen = nullptr;			// generador desde la camara

	SplashGenerator* sGenerator = nullptr;
};

class Scene1 : public Scene
{
public:
	Scene1() = default;
	void init() override;
	void step(double t) override;
	void load() override;
	void unload() override;
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override;
	void specialKeyPressed(int key, const physx::PxTransform& camera) override;

	void newToy(Vector3 pos) override;

	void readFile(std::string file) override;

	void splash(Vector3 pos) override {}

private:
	ForceGenerator* viento = nullptr;
};

class Scene2 : public Scene
{
public:
	Scene2() = default;
	void init() override;
	void step(double t) override;
	void load() override
	{ 
		Scene::load();

		torbellino->setAreaVisibility(true);
		torbellino->toggleAreaVisibility();
	}
	void unload() override
	{
		Scene::unload();

		torbellino->setAreaVisibility(false);
		torbellino->toggleAreaVisibility();
	}
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override;
	void specialKeyPressed(int key, const physx::PxTransform& camera) override {}

	void newToy(Vector3 pos) override {}
	void splash(Vector3 pos) override {}

	void readFile(std::string file) override {}

private:
	ForceGenerator* torbellino = nullptr;
};

// 
class Scene3 : public Scene
{
public:
	Scene3() = default;
	void init() override;
	void step(double t) override;
	void load() override
	{
		Scene::load();

		if (fuegos != nullptr)
		{
			fuegos->setAreaVisibility(true);
			fuegos->toggleAreaVisibility();
		}
	}
	void unload() override
	{
		Scene::unload();

		if (fuegos != nullptr)
		{
			fuegos->setAreaVisibility(false);
			fuegos->toggleAreaVisibility();
		}
	}
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override;
	void specialKeyPressed(int key, const physx::PxTransform& camera) override {}

	void newToy(Vector3 pos) override {}
	void splash(Vector3 pos) override;

	void readFile(std::string file) override {}

private:
	ForceGenerator* fuegos = nullptr;

	ChargedEntity* bola = nullptr;

	SplashGenerator* sGenerator = nullptr;
};

// muelles
class Scene4 : public Scene
{
public:
	Scene4() = default;
	void init() override;
	void step(double t) override;
	void load() override
	{
		Scene::load();
	}
	void unload() override
	{
		Scene::unload();
	}
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override {};
	void specialKeyPressed(int key, const physx::PxTransform& camera) override {}

	void newToy(Vector3 pos) override {}
	void splash(Vector3 pos) override {}

	void readFile(std::string file) override {}

private:
	//ChargedEntity* bola = nullptr;
};

// flotacion
class Scene5 : public Scene
{
public:
	Scene5() = default;
	void init() override;
	void step(double t) override;
	void load() override
	{
		Scene::load();
	}
	void unload() override
	{
		Scene::unload();
	}
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override {};
	void specialKeyPressed(int key, const physx::PxTransform& camera) override {}

	void newToy(Vector3 pos) override {}
	void splash(Vector3 pos) override {}

	void readFile(std::string file) override {}

private:
	ForceGenerator* agua = nullptr;

	ChargedEntity* bola = nullptr;
};

// solidos rigidos
class Scene6 : public Scene
{
public:
	Scene6(PxPhysics* gphys, PxScene* gscn): Scene(gphys, gscn) {}
	void init() override;
	void step(double t) override;
	void load() override
	{
		Scene::load();
	}
	void unload() override
	{
		Scene::unload();
	}
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override {}
	void specialKeyPressed(int key, const physx::PxTransform& camera) override {}

	void newToy(Vector3 pos) override {}
	void splash(Vector3 pos) override {}

	void readFile(std::string file) override {}

private:
	//ForceGenerator* agua = nullptr;

	//ChargedEntity* bola = nullptr;
};