#include <vector>

#include "PxPhysicsAPI.h"

#include "core.hpp"
#include "RenderUtils.hpp"

using namespace physx;

extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive, double t);	
extern void cleanupPhysics(bool interactive);
extern void keyPress(unsigned char key, const PxTransform& camera);
extern void specialKeyPress(int key, const PxTransform& camera); // caracteres especiales
extern PxPhysics* gPhysics;
extern PxMaterial* gMaterial;

std::vector<const RenderItem*> gRenderItems;

double PCFreq = 0.0;
__int64 CounterStart = 0;
__int64 CounterLast = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart) /*/ 1000.0*/;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	CounterLast = CounterStart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t = double(li.QuadPart - CounterLast) / PCFreq;
	CounterLast = li.QuadPart;
	return t;
}

namespace
{
	Camera*	sCamera;

void motionCallback(int x, int y)
{
	sCamera->handleMotion(x, y);
}

void keyboardCallback(unsigned char key, int x, int y)
{
	if(key==27)
		exit(0);

	if(!sCamera->handleKey(key, x, y))
		keyPress(key, sCamera->getTransform());
}

void specialKeyboardCallback(int key, int x, int y)
{
	specialKeyPress(key, sCamera->getTransform());
}

void mouseCallback(int button, int state, int x, int y)
{
	sCamera->handleMouse(button, state, x, y);
}

void idleCallback()
{
	glutPostRedisplay();
}

float stepTime = 0.0f;
//#define FIXED_STEP

void renderCallback()
{
	double t = GetCounter();
#ifdef FIXED_STEP
	if (t < (1.0f / 30.0f))
	{
		fprintf(stderr, "Time: %f\n", stepTime);
		stepTime += t;
	}
	else
		stepTime = 1.0f / 30.0f;

	if (stepTime >= (1.0f / 30.0f))
	{
		stepPhysics(true, stepTime);
		stepTime = 0.0f;
	}
#else
	stepPhysics(true, t);
#endif

	startRender(sCamera->getEye(), sCamera->getDir());

	// renderizar objetos opacos
	//fprintf(stderr, "Num Render Items: %d\n", static_cast<int>(gRenderItems.size()));
	for (auto it = gRenderItems.begin(); it != gRenderItems.end(); ++it)
	{
		const RenderItem* obj = (*it);
		if (obj == nullptr) break;
		auto objTransform = obj->transform;

		if (obj->color.w != 1.0) continue; // si es trasparente pasa, PRIMERO LOS OPACOS

		if (!objTransform)
		{
			auto actor = obj->actor;
			if (actor)
			{
				renderShape(*obj->shape, actor->getGlobalPose(), obj->color);
				continue;
			}
		}
		renderShape(*obj->shape, objTransform ? *objTransform : physx::PxTransform(PxIdentity), obj->color);
	}

	// renderizar objetos transparentes -> recorrer el vecto de atras en adelante para no tapar las particulas
	for (std::vector<const RenderItem*>::reverse_iterator it = gRenderItems.rbegin(); it != gRenderItems.rend(); ++it)
	{
		const RenderItem* obj = (*it);
		if (obj == nullptr) continue;
		auto objTransform = obj->transform;

		if (obj->color.w == 1.0) continue; // si es opaco pasa, DESPUES LOS TRANSPARENTES

		if (!objTransform)
		{
			auto actor = obj->actor;
			if (actor)
			{
				renderShape(*obj->shape, actor->getGlobalPose(), obj->color);
				continue;
			}
		}
		renderShape(*obj->shape, objTransform ? *objTransform : physx::PxTransform(PxIdentity), obj->color);
	}

	//PxScene* scene;
	//PxGetPhysics().getScenes(&scene, 1);
	//PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	//if (nbActors)
	//{
	//	std::vector<PxRigidActor*> actors(nbActors);
	//	scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
	//	renderActors(&actors[0], static_cast<PxU32>(actors.size()), true, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	//}

	finishRender();
}

void exitCallback(void)
{
	delete sCamera;
	cleanupPhysics(true);
}
}

void renderLoop()
{
	StartCounter();
	sCamera = new Camera(
		PxVec3(100.0f, 100.0f, 380.0f),	// eye
		PxVec3(0.0f,0.0f,-1.0f));		// dir

	setupDefaultWindow("SIM");
	setupDefaultRenderState();

	glutIdleFunc(idleCallback);
	glutDisplayFunc(renderCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	glutSpecialFunc(specialKeyboardCallback);
	motionCallback(0,0);

	atexit(exitCallback);

	initPhysics(true);
	glutMainLoop();
}

void RegisterRenderItem(const RenderItem* _item)
{
	gRenderItems.push_back(_item);
}

void DeregisterRenderItem(const RenderItem* _item)
{
	auto it = find(gRenderItems.begin(), gRenderItems.end(), _item);
	if (it == gRenderItems.end()) return; // si no lo encuentra
	gRenderItems.erase(it);
}

double GetLastTime()
{
	double t = double(CounterLast - CounterStart) / PCFreq;
	return t;
}

Camera* GetCamera()
{
	return sCamera;
}

PxShape* CreateShape(const PxGeometry& geo, const PxMaterial* mat)
{
	if (mat == nullptr)
		mat = gMaterial; // Default material

	PxShape* shape = gPhysics->createShape(geo, *mat);
	return shape;
}