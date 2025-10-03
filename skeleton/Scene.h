#pragma once
#include <vector>
#include <foundation/PxTransform.h>

#include "Entity.h"
#include "Particle.h"
class ProjectileGenerator;

class Scene
{
public:
	Scene() = default;
	virtual ~Scene();

	virtual void init();

	virtual void step(double t); // update

	// cambio de escenas
	virtual void load();
	virtual void unload();

	// gestion entidades
	void addEntity(Entity* ent);

	virtual void keyPressed(unsigned char key, const physx::PxTransform& camera);

protected:
	std::vector<Entity*> gObjects;		// Entidades de la escena
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

private:
	ProjectileGenerator* pGenerator = nullptr;
};