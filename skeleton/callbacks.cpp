#include "callbacks.hpp"

extern void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

// A callback function that runs for every potential collision pair, deciding whether to generate a contact, 
// ignore the pair (eKILL / eSUPPRESS), or allow full interaction/callback(eDEFAULT / eCALLBACK)
physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, 
	physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1,
	physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, 
	physx::PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	//PX_UNUSED(filterData0);
	//PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	if ((filterData0.word0 & filterData1.word1) == 0 ||
		(filterData1.word0 & filterData0.word1) == 0)
	{
		return physx::PxFilterFlag::eSUPPRESS;
	} 

	// all initial and persisting reports for everything, with per-point data
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT 
		| physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
		| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return physx::PxFilterFlag::eDEFAULT;
}

void ContactReportCallback::onContact(const physx::PxContactPairHeader& pairHeader, 
	const physx::PxContactPair* pairs, 
	physx::PxU32 nbPairs)
{
	PX_UNUSED(pairs);
	PX_UNUSED(nbPairs);
	physx::PxActor* actor1 = pairHeader.actors[0];
	physx::PxActor* actor2 = pairHeader.actors[1];

	// flags de colision

	onCollision(actor1, actor2);
}