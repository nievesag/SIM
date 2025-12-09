#include "Scene.h"

#include "RenderUtils.hpp"
#include "RigidBodySystem.h"
#include "Wall.h"
class Pipe;
#include <fstream>

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

// ------------------------ ESCENAS HIJAS ------------------------

// -------- ESCENA 0 -> main
void Scene0::init()
{
	Scene::init();

	readFile("mapa1.txt");

	pipe = new Pipe(this, {160,240,0 });
	addEntity(pipe);

	// ----- System
	sys = new ParticleSystem(this);
	RigidBodySystem* sysRb = new RigidBodySystem(this, gPhysics, gScene);

	// -- Particle generators
	trailGenerator = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trailGenerator);

	chargedGenerator = new ChargedRbGenerator(this, "Carga", gPhysics, gScene);
	sysRb->registerGenerator(chargedGenerator);

	ChargedEntity* bola = new ChargedEntity(this, { 0, 0,0 }, 3, -0.1f, trailGenerator, gPhysics, gScene);
	chargedGenerator->addEntity(bola);

	sGenerator = new SplashGenerator(this, "Splash");
	sys->registerGenerator(sGenerator);

	pGen = new ProjectileGenerator(this);

	magnetism1 = new MagnetismGenerator({ 92,38,0 }, 53, this, -0.01);
	sys->registerForceGenerator(magnetism1);
	magnets.push_back(magnetism1);

	magnetism2 = new MagnetismGenerator({ 95,140,0 }, 70, this, 0.2);
	sys->registerForceGenerator(magnetism2);
	magnets.push_back(magnetism2);

	pSystems.push_back(sys);
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

	//DeregisterRenderItem(pipe->getRenderItem());
}

void Scene0::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	//int modifiers = glutGetModifiers();

	//bool ctrl = false;
	//if (modifiers & GLUT_ACTIVE_CTRL) // pulsas el control
	//{
	//	ctrl = true;
	//}

	int modifiers = glutGetModifiers();

	if (modifiers & GLUT_ACTIVE_CTRL && key == 17) // pulsar control && q
	{
		selectedMagnet = magnets[1];
		std::cout << "iman " << 0 << " seleccionado" << std::endl;
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
	/*
	ChargedEntity* toy = new ChargedEntity(this, pos, 3, -0.1, trail);
	toy->setVelocity({ 0,-50,0 });

	ChargedGenerator* chargedGenerator = new ChargedGenerator(this, "Carga");
	sys->registerGenerator(chargedGenerator);
	chargedGenerator->addChargedEnitity(toy);

	// pie
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

			/*
			// MURO
			if (fila[j] == 'x')
			{
				Wall* wall = new Wall(this, 20, Vector3( j * 40, i * 40,0  ), false);
				line.push_back(wall);
			}
			// VACIO
			else if (fila[j] == 'o')
			{
				Wall* empty = new Wall(this, 20, Vector3(j * 40, i * 40, 0), true);
				line.push_back(empty);
			}
			*/
		}

		map.push_back(line);
	}
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

	ParticleSystem* sys = new ParticleSystem(this);

	ParticleGenerator* randomMass = new RandomMassGenerator(this, "Cascada");
	sys->registerGenerator(randomMass);

	viento = new WindGenerator({ 0,0,0 }, 50, this, { 40, 0, 0 });
	sys->registerForceGenerator(viento);

	pSystems.push_back(sys);
}

void Scene1::step(double t)
{
	Scene::step(t);
}

void Scene1::load()
{
	Scene::load();

	viento->setAreaVisibility(true);
	viento->toggleAreaVisibility();
}

void Scene1::unload()
{
	Scene::unload();

	viento->setAreaVisibility(false);
	viento->toggleAreaVisibility();
}

void Scene1::keyPressed(unsigned char key, const physx::PxTransform& camera)
{
	switch (toupper(key))
	{
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
}

void Scene1::newToy(Vector3 pos)
{
}

void Scene1::readFile(std::string file)
{
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

	//TrailGenerator* trail = new TrailGenerator(this, "Rastro");
	//sys->registerGenerator(trail);

	SpringParticleGenerator* springGenerator = new SpringParticleGenerator(this, "Carga");
	sys->registerGenerator(springGenerator);

	// crea toy
	/*
	ChargedEntity* toy = new ChargedEntity(this, {0,0,0}, 3, -0.1, nullptr);
	toy->setLifetime(-1);
	toy->setVelocity({ 0,-50,0 });

	ChargedGenerator* chargedGenerator = new ChargedGenerator(this, "Carga");
	sys->registerGenerator(chargedGenerator);
	chargedGenerator->addChargedEnitity(toy);

	// pie
	Particle* pie = new Particle(this, {0,0,0}, {0,0,0}, 2, {0,0,1,1}, 1.5, 0.99, -1);
	addEntity(pie);
	springGenerator->addSpringEnitity(toy);
	springGenerator->addSpringEnitity(pie);

	// muelles
	SpringForceGenerator* spring1 = new SpringForceGenerator(1, 10, pie);
	sys->registerForceGenerator(spring1);
	SpringForceGenerator* spring2 = new SpringForceGenerator(1, 10, toy);
	sys->registerForceGenerator(spring2);
	*/

	pSystems.push_back(sys);
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

	//TrailGenerator* trail = new TrailGenerator(this, "Rastro");
	//sys->registerGenerator(trail);

	//SpringParticleGenerator* springGenerator = new SpringParticleGenerator(this, "Carga");
	//sys->registerGenerator(springGenerator);

	// crea toy
	/*
	ChargedEntity* toy = new ChargedEntity(this, { 0,0,0 }, 3, -0.1, nullptr);
	toy->setMass(1);
	toy->setLifetime(-1);
	
	ChargedGenerator* chargedGenerator = new ChargedGenerator(this, "Carga");
	sys->registerGenerator(chargedGenerator);
	chargedGenerator->addChargedEnitity(toy);*/

	// agua
	BuoyancyForceGenerator* agua = new BuoyancyForceGenerator({ 0,0,0 }, 50, this, -10, 5, 1);
	sys->registerForceGenerator(agua);

	pSystems.push_back(sys);
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
	gScene->addActor(*pared->getActor());
	addEntity(pared);

	RigidBodySystem* sys = new RigidBodySystem(this, gPhysics, gScene);

	RigidBodyGenerator* rbGen = new RigidBodyGenerator(this, "Cascada", gPhysics, gScene);
	sys->registerGenerator(rbGen);

	RigidBodyDynamic* bola = new RigidBodyDynamic(this, gPhysics, gScene);
	bola->setMass(10);
	bola->setLinearVelocity({ 0,5,0 });
	bola->setAngularVelocity({ 0,0,0 });
	gScene->addActor(*bola->getActor());

	bola->setPosition({0,100,0});
	bola->setDensity(1);
	//rbGen->addEntity(bola);
	addEntity(bola);

	// Scene* scn, PxPhysics* gPhysics, PxScene* gScene, PxMaterial* mat, 
	//bool kin, Vector3 pos, Vector3 vel, double siz, PxVec3 vol, Vector4 col, float m, float damp, float maxLT,
	//	Shape sh, double d, PxVec3 angVel, PxVec3 tensor

	pSystems.push_back(sys);
}

void Scene6::step(double t)
{
	Scene::step(t);
}