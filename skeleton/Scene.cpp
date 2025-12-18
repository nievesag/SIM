#include "Scene.h"

#include "RenderUtils.hpp"
#include "RigidBodySystem.h"
#include "Wall.h"
#include "Win.h"
class Pipe;
#include <fstream>
#include "ChargedEntity.h"

Scene::Scene(PxPhysics* gPhysics, PxScene* gScene)
	: gPhysics(gPhysics), gScene(gScene)
{

}

Scene::~Scene()
{

}

void Scene::init()
{

}

void Scene::step(double t) // update
{
	deleteEntities();

	for (auto e : gObjects)
	{
		e->step(t);
	}

	for (auto s : pSystems)
	{
		s->step(t);
	}
}

void Scene::deleteEntities()
{
	for (auto g : gObjects)
	{
		if (!g->getAlive())
		{
			auto it = std::find(gObjects.begin(), gObjects.end(), g);

			if (it != gObjects.end())
			{
				delete g;
				gObjects.erase(it);
			}
		}
	}
}

void Scene::load()
{
	for (auto e : gObjects)
	{
		e->setVisible(true);
		e->toggleVisibility();
	}
}

void Scene::unload()
{
	// para eliminarlos de la escena
	for (auto e : gObjects)
	{
		e->setVisible(false);
		e->toggleVisibility();
	}
}

void Scene::addEntity(Entity* ent)
{
	gObjects.push_back(ent);
}

void Scene::addSystem(System* sys)
{
	pSystems.push_back(sys);
}

void Scene::keyPressed(unsigned char key, const physx::PxTransform& camera)
{

}

void Scene::specialKeyPressed(int key, const physx::PxTransform& camera)
{

}

void Scene::readFile(std::string file)
{
	std::ifstream entrada(file);

	if (!entrada.is_open())
	{
		std::cout << "Error abriendo archivo\n";
		return;
	}

	// para poder hacer cin para leer el archivo
	auto cinbuf = std::cin.rdbuf(entrada.rdbuf());

	std::cin >> height >> width;  // mapa

	std::string fila;
	std::vector<Wall*> line;

	for (int i = 0; i < height; i++)
	{
		std::cin >> fila;

		for (int j = 0; j < width; j++)
		{
			std::cout << j << " " << i << std::endl;

			// MURO
			if (fila[j] == 'x')
			{
				Wall* wall = new Wall(this, 20.0f, Vector3(j * 40, i * 40, 0), false, gPhysics, gScene);
				map.push_back(wall);
			}
			// VACIO
			else if (fila[j] == 'w')
			{
				Win* win = new Win(this, 20.0f, Vector3(j * 40, i * 40, 0), gPhysics, gScene);
				map.push_back(win);
			}
		}
	}
}

// ------------------------ ESCENAS HIJAS ------------------------
// -------- ESCENA 0 -> NIVEL 1
void Scene0::init()
{
	Scene::init();

	readFile("mapa1.txt");

	pipe = new Pipe(this, {160,240,0 });
	addEntity(pipe);

	// ----- System
	sys = new ParticleSystem(this);
	sysRb = new RigidBodySystem(this, gPhysics, gScene);

	// -- Particle generators
	trailGenerator = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trailGenerator);

	chargedGenerator = new ChargedRbGenerator(this, "Carga", gPhysics, gScene);
	sysRb->registerGenerator(chargedGenerator);

	sGenerator = new SplashGenerator(this, "Splash");
	sys->registerGenerator(sGenerator);

	pGen = new ProjectileGenerator(this);

	magnetism1 = new MagnetismGenerator({ 92,38,0 }, 53, this, -0.01, gPhysics, gScene);
	sysRb->registerForceGenerator(magnetism1);
	magnets.push_back(magnetism1);

	magnetism2 = new MagnetismGenerator({ 95,140,0 }, 70, this, 0.2, gPhysics, gScene);
	sysRb->registerForceGenerator(magnetism2);
	magnets.push_back(magnetism2);

	pSystems.push_back(sys);
	pSystems.push_back(sysRb);
}

void Scene0::step(double t)
{
	Scene::step(t);

	if (!fatherPart)
	{
		fatherPart = true;
	}
}

void Scene0::load()
{
	Scene::load();

	magnetism1->setAreaVisibility(true);
	magnetism2->setAreaVisibility(true);
	magnetism1->toggleAreaVisibility();
	magnetism2->toggleAreaVisibility();
	magnetism1->toggleMagnetVisibility();
	magnetism2->toggleMagnetVisibility();

	//RegisterRenderItem(pipe->getRenderItem());
}

void Scene0::unload()
{
	Scene::unload();

	magnetism1->setAreaVisibility(false);
	magnetism2->setAreaVisibility(false);

	magnetism1->toggleAreaVisibility();
	magnetism2->toggleAreaVisibility();

	magnetism1->toggleMagnetVisibility();
	magnetism2->toggleMagnetVisibility();

	chargedGenerator->unloadGenerator();

	for (auto i : map)
	{
		i->setVisible(false);
		i->toggleVisibility();
		gScene->removeActor(*i->getActor());
	}
	map.clear();

	//DeregisterRenderItem(pipe->getRenderItem());
}

void Scene0::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	int modifiers = glutGetModifiers();

	if (modifiers & GLUT_ACTIVE_CTRL && key == 17) // pulsar control && q
	{
		selectedMagnet = magnets[0];
		std::cout << "iman " << 0 << " seleccionado" << std::endl;
	}
	if (modifiers & GLUT_ACTIVE_CTRL && key == 18) // pulsar control && r
	{
		selectedMagnet = magnets[1];
		std::cout << "iman " << 1 << " seleccionado" << std::endl;
	}

	switch(toupper(key))
	{
	case 'Z':
	{
		if (selectedMagnet != nullptr) selectedMagnet->toggleForce();
		break;
	}
	case 'N':
	{
		pipe->eject();
		break;
	}
	case 'I':
	{
		std::cout << "disparo desde la camara" << std::endl;
		if (pGen != nullptr)
		{
			pGen->shoot("Cannon");
		}
		break;
	}
	default:
		break;
	}
}

void Scene0::specialKeyPressed(int key, const physx::PxTransform& camera)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (selectedMagnet != nullptr) selectedMagnet->move({ 0,1.0f,0 });
		break;
	case GLUT_KEY_DOWN:
		if (selectedMagnet != nullptr) selectedMagnet->move({ 0,-1.0f,0 });
		break;
	case GLUT_KEY_LEFT:
		if (selectedMagnet != nullptr) selectedMagnet->move({ -1.0f,0,0 });
		break;
	case GLUT_KEY_RIGHT:
		if (selectedMagnet != nullptr) selectedMagnet->move({ 1.0f,0,0 });
		break;
	}
}

void Scene0::newToy(Vector3 pos)
{
	TrailGenerator* trail = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trail);

	SpringParticleGenerator* springGenerator = new SpringParticleGenerator(this, "Carga");
	sys->registerGenerator(springGenerator);

	// crea toy
	ChargedEntity* toy = new ChargedEntity(this, pos, 3, -0.1f, trailGenerator, gPhysics, gScene);
	toy->setLinearVelocity({ 0,-50,0 });
	ChargedRbGenerator* chargedGenerator = new ChargedRbGenerator(this, "Carga", gPhysics, gScene);
	sysRb->registerGenerator(chargedGenerator);
	chargedGenerator->addEntity(toy);

	// pie
	/*
	Particle* pie = new Particle(this, pos, { 0,0,0 }, 2, { 0,0,1,1 }, 1.5, 0.99, -1);
	addEntity(pie);
	springGenerator->addSpringEnitity(toy);
	springGenerator->addSpringEnitity(pie);

	// muelles
	SpringForceGenerator* spring1 = new SpringForceGenerator(500, 5, pie);
	sys->registerForceGenerator(spring1);
	SpringForceGenerator* spring2 = new SpringForceGenerator(500, 5, toy);
	sys->registerForceGenerator(spring2);
	*/
}

void Scene0::readFile(std::string file)
{
	Scene::readFile(file);
}

void Scene0::splash(Vector3 pos)
{
	sGenerator->setSplasPos(pos);
	sGenerator->setSplash(true);
}

// -------- ESCENA 1 -> viento
void Scene1::init()
{
	Scene::init();

	readFile("mapa2.txt");

	// ----- System
	sys = new ParticleSystem(this);
	sysB = new ParticleSystem(this);
	sysRb = new RigidBodySystem(this, gPhysics, gScene);

	pipe = new Pipe(this, { 200,240,0 });
	addEntity(pipe);

	// -- Particle generators
	ParticleGenerator* randomMass = new RandomMassGenerator(this, "Niebla");
	static_cast<RandomMassGenerator*>(randomMass)->setOrigin({220, 160, 0});
	sys->registerGenerator(randomMass);

	trailGenerator = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trailGenerator);

	chargedGenerator = new ChargedRbGenerator(this, "Carga", gPhysics, gScene);
	sysRb->registerGenerator(chargedGenerator);

	sGenerator = new SplashGenerator(this, "Splash");
	sys->registerGenerator(sGenerator);

	ParticleGenerator* wGenerator = new WaterfallGenerator(this, "Cascada");
	static_cast<WaterfallGenerator*>(wGenerator)->setOrigin({ -105, -150, 0 });
	sysB->registerGenerator(wGenerator);

	pGen = new ProjectileGenerator(this);

	// -- Force generators
	viento = new WindGenerator({ 220, 160, 0 }, 26, this, { -10, 0, 0 });
	sysRb->registerForceGenerator(viento);
	sys->registerForceGenerator(viento);

	magnetism1 = new MagnetismGenerator({ 92,38,0 }, 53, this, -0.01, gPhysics, gScene);
	sysRb->registerForceGenerator(magnetism1);
	magnets.push_back(magnetism1);

	magnetism2 = new MagnetismGenerator({ 95,140,0 }, 70, this, 0.2, gPhysics, gScene);
	sysRb->registerForceGenerator(magnetism2);
	magnets.push_back(magnetism2);

	BuoyancyForceGenerator* agua = new BuoyancyForceGenerator({ 160,140,0 }, 50, { 60,0,5 }, this, 20, 10, 3);
	sysRb->registerForceGenerator(agua);

	pSystems.push_back(sys);
	pSystems.push_back(sysB);
	pSystems.push_back(sysRb);
}

void Scene1::step(double t)
{
	Scene::step(t);

	if (!fatherPart)
	{
		fatherPart = true;
	}
}

void Scene1::load()
{
	Scene::load();

	viento->setAreaVisibility(true);
	viento->toggleAreaVisibility();

	magnetism1->setAreaVisibility(true);
	magnetism2->setAreaVisibility(true);
	magnetism1->toggleAreaVisibility();
	magnetism2->toggleAreaVisibility();
	magnetism1->toggleMagnetVisibility();
	magnetism2->toggleMagnetVisibility();

	//RegisterRenderItem(pipe->getRenderItem());
}

void Scene1::unload()
{
	Scene::unload();

	Scene::unload();

	magnetism1->setAreaVisibility(false);
	magnetism2->setAreaVisibility(false);

	magnetism1->toggleAreaVisibility();
	magnetism2->toggleAreaVisibility();

	magnetism1->toggleMagnetVisibility();
	magnetism2->toggleMagnetVisibility();

	chargedGenerator->unloadGenerator();

	for (auto i : map)
	{
		i->setVisible(false);
		i->toggleVisibility();
		gScene->removeActor(*i->getActor());
	}
	map.clear();

	//DeregisterRenderItem(pipe->getRenderItem());

	viento->setAreaVisibility(false);
	viento->toggleAreaVisibility();
}

void Scene1::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	int modifiers = glutGetModifiers();

	if (modifiers & GLUT_ACTIVE_CTRL && key == 17) // pulsar control && q
	{
		selectedMagnet = magnets[0];
		std::cout << "iman " << 0 << " seleccionado" << std::endl;
	}
	if (modifiers & GLUT_ACTIVE_CTRL && key == 18) // pulsar control && r
	{
		selectedMagnet = magnets[1];
		std::cout << "iman " << 1 << " seleccionado" << std::endl;
	}

	switch (toupper(key))
	{
	case 'Z':
	{
		if (selectedMagnet != nullptr) selectedMagnet->toggleForce();
		break;
	}
	case 'N':
	{
		pipe->eject();
		break;
	}
	case 'I':
	{
		std::cout << "disparo desde la camara" << std::endl;
		if (pGen != nullptr)
		{
			pGen->shoot("Cannon");
		}
		break;
	}
	case 'R':
	{
		if (viento != nullptr) viento->toggleForce();
		break;
	}
	default:
		break;
	}
}

void Scene1::specialKeyPressed(int key, const physx::PxTransform& camera)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (selectedMagnet != nullptr) selectedMagnet->move({ 0,1.0f,0 });
		break;
	case GLUT_KEY_DOWN:
		if (selectedMagnet != nullptr) selectedMagnet->move({ 0,-1.0f,0 });
		break;
	case GLUT_KEY_LEFT:
		if (selectedMagnet != nullptr) selectedMagnet->move({ -1.0f,0,0 });
		break;
	case GLUT_KEY_RIGHT:
		if (selectedMagnet != nullptr) selectedMagnet->move({ 1.0f,0,0 });
		break;
	}
}

void Scene1::newToy(Vector3 pos)
{
	TrailGenerator* trail = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trail);

	SpringParticleGenerator* springGenerator = new SpringParticleGenerator(this, "Carga");
	sys->registerGenerator(springGenerator);

	// crea toy
	ChargedEntity* toy = new ChargedEntity(this, pos, 3, -0.1f, trailGenerator, gPhysics, gScene);
	toy->setLinearVelocity({ 0,-1,0 });
	ChargedRbGenerator* chargedGenerator = new ChargedRbGenerator(this, "Carga", gPhysics, gScene);
	sysRb->registerGenerator(chargedGenerator);
	chargedGenerator->addEntity(toy);

	// pie
	/*
	Particle* pie = new Particle(this, pos, { 0,0,0 }, 2, { 0,0,1,1 }, 1.5, 0.99, -1);
	addEntity(pie);
	springGenerator->addSpringEnitity(toy);
	springGenerator->addSpringEnitity(pie);

	// muelles
	SpringForceGenerator* spring1 = new SpringForceGenerator(500, 5, pie);
	sys->registerForceGenerator(spring1);
	SpringForceGenerator* spring2 = new SpringForceGenerator(500, 5, toy);
	sys->registerForceGenerator(spring2);
	*/
}

void Scene1::readFile(std::string file)
{
	Scene::readFile(file);
}

void Scene1::splash(Vector3 pos)
{
	sGenerator->setSplasPos(pos);
	sGenerator->setSplash(true);
}

// -------- ESCENA 2 -> torbellino
void Scene2::init()
{
	Scene::init();

	ParticleSystem* sys = new ParticleSystem(this);

	ParticleGenerator* wGenerator = new WaterfallGenerator(this, "Cascada");
	sys->registerGenerator(wGenerator);

	torbellino = new WhirlGenerator({ 0,0,0 }, 100, this, { 0, -50, 50 });
	sys->registerForceGenerator(torbellino);

	pSystems.push_back(sys);
}

void Scene2::step(double t)
{
	Scene::step(t);
}

void Scene2::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	switch (toupper(key))
	{
	case 'R':
	{
		if (torbellino != nullptr) torbellino->toggleForce();
		break;
	}
	default:
		break;
	}
}

// -------- ESCENA 3 -> splash
void Scene3::init()
{
	Scene::init();

	ParticleSystem* sys = new ParticleSystem(this);

	// -- Particle generators
	TrailGenerator* trailGenerator = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trailGenerator);

	ChargedGenerator* chargedGenerator = new ChargedGenerator(this, "Carga");
	sys->registerGenerator(chargedGenerator);

	/*
	bola = new ChargedEntity(this, { 140, 200,0 }, 3, -0.1, trailGenerator);
	chargedGenerator->addChargedEnitity(bola);
	*/

	sGenerator = new SplashGenerator(this, "Splash");
	sys->registerGenerator(sGenerator);

	pSystems.push_back(sys);
}

void Scene3::step(double t)
{
	Scene::step(t);
}

void Scene3::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
}

void Scene3::splash(Vector3 pos)
{
	sGenerator->setSplasPos(pos);
	sGenerator->setSplash(true);
}

// -------- ESCENA 4 -> muelles
void Scene4::init()
{
	Scene::init();

	ParticleSystem* sys = new ParticleSystem(this);
	RigidBodySystem* sysRb = new RigidBodySystem(this, gPhysics, gScene);

	ChargedRbGenerator* chargedGenerator = new ChargedRbGenerator(this, "Carga", gPhysics, gScene);
	sysRb->registerGenerator(chargedGenerator);

	TrailGenerator* trailGenerator = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trailGenerator);

	ChargedEntity* cuerpo = new ChargedEntity(this, { 0, 10,0 }, 3, -0.1f, trailGenerator, gPhysics, gScene);
	cuerpo->setLinearVelocity({ 10,0,0 });
	chargedGenerator->addEntity(cuerpo);

	// cuerpo
	RigidBodyDynamic* pie = new RigidBodyDynamic(this, gPhysics, gScene, nullptr, false, { 0,15,0 }, 
		{ 0,0,0 }, 3, { 3,3,3 }, { 0, 0.90f, 0.90f ,1 }, 0.5, 0.8, 
		-1, SPHERE, -1, { 0, 0, 0 }, { 1, 1, 1 },BODYPARTS);
	pie->setColor({ 1,0,0,1 });
	chargedGenerator->addEntity(pie);

	// muelles
	SpringForceGenerator* spring1 = new SpringForceGenerator(10, 0.1, pie);
	sysRb->registerForceGenerator(spring1);

	SpringForceGenerator* spring2 = new SpringForceGenerator(10, 0.1, cuerpo);
	sysRb->registerForceGenerator(spring2);

	pSystems.push_back(sys);
	pSystems.push_back(sysRb);
}

void Scene4::step(double t)
{
	Scene::step(t);
}

// -------- ESCENA 5 -> flotacion
void Scene5::init()
{
	Scene::init();

	ParticleSystem* sys = new ParticleSystem(this);
	RigidBodySystem* sysRb = new RigidBodySystem(this, gPhysics, gScene);

	// crea toy
	ChargedRbGenerator* chargedGenerator = new ChargedRbGenerator(this, "Carga", gPhysics, gScene);
	sysRb->registerGenerator(chargedGenerator);

	TrailGenerator* trailGenerator = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trailGenerator);

	ChargedEntity* bola = new ChargedEntity(this, { 0, 100,0 }, 3, -0.1f, trailGenerator, gPhysics, gScene);
	bola->setColor({ 1,0,0,1 });
	chargedGenerator->addEntity(bola);

	// agua
	BuoyancyForceGenerator* agua = new BuoyancyForceGenerator({ 0,0,0 }, 50, {10,0,10}, this, 10, 5, 1);
	sysRb->registerForceGenerator(agua);

	pSystems.push_back(sys);
	pSystems.push_back(sysRb);
}

void Scene5::step(double t)
{
	Scene::step(t);
}

// -------- ESCENA 5 -> solidos rigidos
void Scene6::init()
{
	Scene::init();

	Wall* pared = new Wall(this, 5.0f, { 0,0,0 }, false, gPhysics, gScene);
	//gScene->addActor(*pared->getActor());
	addEntity(pared);

	RigidBodySystem* sys = new RigidBodySystem(this, gPhysics, gScene);

	RigidBodyGenerator* rbGen = new RigidBodyGenerator(this, "Cascada", gPhysics, gScene);
	sys->registerGenerator(rbGen);

	RigidBodyDynamic* bola = new RigidBodyDynamic(this, gPhysics, gScene);
	bola->setMass(10);
	//bola->setLinearVelocity({ 0,5,0 });
	bola->setAngularVelocity({ 0,0,0 });
	//gScene->addActor(*bola->getActor());

	bola->setPosition({0,100,0});
	bola->setDensity(1);
	//rbGen->addEntity(bola);
	addEntity(bola);

	pSystems.push_back(sys);
}

void Scene6::step(double t)
{
	Scene::step(t);
}