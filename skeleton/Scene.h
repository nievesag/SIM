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
	virtual void readFile(std::string file);
	virtual void splash(Vector3 pos) = 0;

	// px
	PxPhysics* getPxPhysics() const	{ return gPhysics; }
	PxScene* getPxScene() const { return gScene; }

	void endLevel() { _endLevel = true; }
	bool getEndLevel() { return _endLevel; }

	virtual void explosion(Vector3 pos) {}

protected:
	std::vector<Entity*> gObjects;		// Entidades de la escena
	std::vector<System*> pSystems; // sistemas de particulas

	Vector3 actionThreshold = {10000, 10000,10000 }; // limites de la escena a partir de los cuales se eliminan las particulas
	
	PxPhysics* gPhysics = nullptr;
	PxScene* gScene = nullptr;

	// nivel
	int width = 0, height = 0; // tamaño del mapa
	std::vector<RigidBody*> map;

	bool _endLevel = false;
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

	void explosion(Vector3 pos) override {}

private:
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

	SplashGenerator* sGenerator = nullptr;
};

class Scene1 : public Scene
{
public:
	Scene1() = default;
	Scene1(PxPhysics* gphys, PxScene* gscn) : Scene(gphys, gscn) {}
	void init() override;
	void step(double t) override;
	void load() override;
	void unload() override;
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override;
	void specialKeyPressed(int key, const physx::PxTransform& camera) override;

	void newToy(Vector3 pos) override;

	void readFile(std::string file) override;
	void splash(Vector3 pos) override;

	void explosion(Vector3 pos) override {}

private:
	ParticleSystem* sys = nullptr; // sistema de particulas
	ParticleSystem* sysB = nullptr; // para el agua (visual)
	RigidBodySystem* sysRb = nullptr;

	std::vector<MagnetismGenerator*> magnets; // vector de todos los imanes
	MagnetismGenerator* magnetism1 = nullptr; // iman 1
	MagnetismGenerator* magnetism2 = nullptr; // iman 2
	BuoyancyForceGenerator* water = nullptr; // iman 2

	MagnetismGenerator* selectedMagnet = nullptr;	// iman que tienes seleccionado

	ChargedRbGenerator* chargedGenerator = nullptr;	// generador de particulas con carga
	TrailGenerator* trailGenerator = nullptr;		// rastro que deja la particula
	bool fatherPart = false;

	Pipe* pipe = nullptr; // tuberia que suelta la particula

	ForceGenerator* viento = nullptr;

	SplashGenerator* sGenerator = nullptr;
};

class Scene2 : public Scene
{
public:
	Scene2() = default;
	Scene2(PxPhysics* gphys, PxScene* gscn) : Scene(gphys, gscn) {}
	void init() override;
	void step(double t) override;
	void load() override;
	void unload() override;
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override;
	void specialKeyPressed(int key, const physx::PxTransform& camera) override;

	void newToy(Vector3 pos) override;

	void readFile(std::string file) override;
	void splash(Vector3 pos) override;

	void explosion(Vector3 pos) override {}

private:
	ParticleSystem* sys = nullptr; // sistema de particulas
	ParticleSystem* sysB = nullptr; // sistema de particulas
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
	ForceGenerator* firework = nullptr;
	ParticleGenerator* randomMass = nullptr;

	ProjectileGenerator* pGen = nullptr;			// generador desde la camara

	SplashGenerator* sGenerator = nullptr;
	FireworkGenerator* eGenerator = nullptr;

	float timer = 0;
	float torbellinoTime = 10;
};

// FINAL
class Scene3 : public Scene
{
public:
	Scene3() = default;
	Scene3(PxPhysics* gphys, PxScene* gscn) : Scene(gphys, gscn) {}
	void init() override;
	void step(double t) override;
	void load() override;
	void unload() override;
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override;
	void specialKeyPressed(int key, const physx::PxTransform& camera) override {}

	void newToy(Vector3 pos) override {}

	void readFile(std::string file) override {}
	void splash(Vector3 pos) override {}

	void explosion(Vector3 pos) override;

private:
	ParticleSystem* sysB = nullptr; // sistema de particulas

	ForceGenerator* firework = nullptr;

	ProjectileGenerator* pGen = nullptr;			// generador desde la camara

	FireworkGenerator* eGenerator = nullptr;
};

// muelles
class Scene4 : public Scene
{
public:
	Scene4() = default;
	Scene4(PxPhysics* gphys, PxScene* gscn) : Scene(gphys, gscn) {}

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
	Scene5(PxPhysics* gphys, PxScene* gscn) : Scene(gphys, gscn) {}

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