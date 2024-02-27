#ifndef __PYENGINE_2_0_PHYSICSMANAGER_H__
#define __PYENGINE_2_0_PHYSICSMANAGER_H__

#define NOMINMAX
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "PrimeEngine/MemoryManagement/Handle.h"
// #include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "../Events/Component.h"
#include "../Utils/Array/Array.h"
#include "PrimeEngine/Scene/PhysicsComponent.h"

namespace PE {
namespace Components{

struct PhysicsManager : public Component
{
	PE_DECLARE_CLASS(PhysicsManager);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_PRE_PHYSICS_UPDATE);
	virtual void do_PRE_PHYSICS_UPDATE(Events::Event* pEvt);
	
	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_POST_PHYSICS_UPDATE);
	virtual void do_POST_PHYSICS_UPDATE(Events::Event* pEvt);

	PhysicsManager(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself);
	virtual ~PhysicsManager(){}

	// Singleton ------------------------------------------------------------------
	void addDefaultComponents();
	
    Array<Handle> m_physicsComponentList;


};

}; // namespace Components
}; // namespace PE
#endif
