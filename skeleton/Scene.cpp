#include "Scene.h"

#include "ProjectileGenerator.h"
#include "RenderUtils.hpp"
#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "Wall.h"
class Pipe;
#include <fstream>

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
	
}

void Scene::unload()
{
	// para eliminarlos de la escena
	for (auto e : gObjects)
		delete e;
	gObjects.clear();
}

void Scene::addEntity(Entity* ent)
{
	gObjects.push_back(ent);
}

void Scene::addSystem(ParticleSystem* sys)
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
// Escena 0
void Scene0::init()
{
	Scene::init();

	readFile("mapa1.txt");

	pipe = new Pipe(this, { 30,100,0 });
	addEntity(pipe);

	// ----- System
	sys = new ParticleSystem(this);

	// -- Particle generators
	//ParticleGenerator* waterfallGenerator = new WaterfallGenerator(this, "Cascada");
	//sys->registerGenerator(waterfallGenerator);

	//ParticleGenerator* mistGenerator = new MistGenerator(this, "Niebla");
	//sys->registerGenerator(mistGenerator);

	//ParticleGenerator* fireworkGenerator = new FireworkGenerator(this, "Fuegos");
	//sys->registerGenerator(fireworkGenerator);

	trailGenerator = new TrailGenerator(this, "Rastro");
	sys->registerGenerator(trailGenerator);

	chargedGenerator = new ChargedGenerator(this, "Carga");
	sys->registerGenerator(chargedGenerator);

	ChargedEntity* bola = new ChargedEntity(this, { 40, 200,0 }, 3, -0.1, trailGenerator);
	chargedGenerator->addChargedEnitity(bola);

	pGen = new ProjectileGenerator(GetCamera()->getEye(), GetCamera()->getDir().getNormalized() * 0.2, 5, "h", this);

	/*
	// -- Force generators
	//ForceGenerator* gg = new GravityGenerator({ 0,0,0 }, 50, this, {0, -9.8, 0});
	//sys->registerForceGenerator(gg);

	//ForceGenerator* fg = new WhirlGenerator({ 0,0,0 }, 50, this, { 0, -10, 20 });
	//sys->registerForceGenerator(fg);
	*/

	//ForceGenerator* fg = new WindGenerator({ 0,0,0 }, 50, this, { 0, -10, 20 });
	//sys->registerForceGenerator(fg);

	magnetism1 = new MagnetismGenerator({ -50,100,0 }, 50, this, -0.01);
	sys->registerForceGenerator(magnetism1);
	magnets.push_back(magnetism1);

	magnetism2 = new MagnetismGenerator({ 50,100,0 }, 50, this, 0.2);
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
}

void Scene0::unload()
{
	Scene::unload();
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
		selectedMagnet->setPos(map[0][0]->getPosition());
		std::cout << "iman " << 0 << " seleccionado" << std::endl;
	}

	switch(toupper(key))
	{
	case 'Z':
	{
		magnetism1->toggleForce();
		break;
	}
	case 'N':
	{
		pipe->eject();
		break;
	}
	case 'I':
	{
		pGen->shoot();
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

	ChargedEntity* toy = new ChargedEntity(this, pos, 3, -0.1, trail);
	toy->setVelocity({ 0,-100,0 }); // ?????????
	//toy->addForce({ 0,200,0 }); // ?????????
	//toy->setAcc({ 0,200,0 });

	chargedGenerator->addChargedEnitity(toy);
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
		}

		map.push_back(line);
	}
}