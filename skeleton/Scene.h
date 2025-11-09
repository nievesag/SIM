#pragma once
#include <vector>
#include <foundation/PxTransform.h>

#include "Entity.h"
#include "Particle.h"
#include "ForceGenerator.h"
#include "ParticleGenerator.h"
#include "ChargedEntity.h"
#include "Pipe.h"
class Wall;

class ParticleSystem;

class Scene
{
public:
	Scene() = default;
	virtual ~Scene();

	virtual void init();

	virtual void step(double t); // update

	void deleteEntities();

	// cambio de escenas
	virtual void load();
	virtual void unload();

	// gestion entidades
	void addEntity(Entity* ent);
	void addSystem(ParticleSystem* sys);

	virtual void keyPressed(unsigned char key, const physx::PxTransform& camera);
	virtual void specialkey(int key, const physx::PxTransform& camera); // special keypress event

	Vector3 getActionThreshold() { return actionThreshold; }

	virtual void newToy(Vector3 pos) = 0;
	virtual void readFile(std::string file) = 0;

protected:
	std::vector<Entity*> gObjects;		// Entidades de la escena
	std::vector<ParticleSystem*> pSystems; // sistemas de particulas

	Vector3 actionThreshold = {200, 200,200}; // limites de la escena a partir de los cuales se eliminan las particulas
};

// --- ESCENAS HIJAS ---
// Scene0, Scene1, Scene2 ...

class Scene0 : public Scene
{
public:
	Scene0() = default;
	void init() override;
	void step(double t) override;
	void load() override;
	void unload() override;
	void keyPressed(unsigned char key, const physx::PxTransform& camera) override;
	void specialkey(int key, const physx::PxTransform& camera) override;

	void newToy(Vector3 pos) override;

	void readFile(std::string file) override;

private:
	ParticleSystem* sys = nullptr;

	std::vector<std::vector<Wall*>> map;

	std::vector<MagnetismGenerator*> magnets;

	MagnetismGenerator* magnetism1 = nullptr;
	MagnetismGenerator* magnetism2 = nullptr;

	MagnetismGenerator* selectedMagnet = nullptr;

	ChargedGenerator* chargedGenerator = nullptr;

	TrailGenerator* trailGenerator = nullptr;

	bool fatherPart = false;

	int width = 0, height = 0;

	Pipe* pipe = nullptr;
};