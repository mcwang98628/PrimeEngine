// APIAbstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes

// Inter-Engine includes
#include "../Lua/LuaEnvironment.h"

// Sibling/Children includes
#include "PhysicsManager.h"
#include "../Sound/SoundManager.h"
#include "PrimeEngine/Events/StandardEvents.h"

#include "PrimeEngine/Scene/Skeleton.h"
#include "PrimeEngine/Scene/SceneNode.h"
#include "PrimeEngine/Scene/Collider.h"

namespace PE {
namespace Components {

using namespace PE::Events;

PE_IMPLEMENT_CLASS1(PhysicsManager, Component);

// Constructor -------------------------------------------------------------
PhysicsManager::PhysicsManager(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself)
: Component(context, arena, hMyself), m_physicsComponentList(context, arena, 256)
{
	
}

void PhysicsManager::addDefaultComponents()
{
	PE_REGISTER_EVENT_HANDLER(Events::Event_PRE_PHYSICS_UPDATE, PhysicsManager::do_PRE_PHYSICS_UPDATE);
	PE_REGISTER_EVENT_HANDLER(Events::Event_POST_PHYSICS_UPDATE, PhysicsManager::do_POST_PHYSICS_UPDATE);
}

void PhysicsManager::do_PRE_PHYSICS_UPDATE(Events::Event* pEvt)
{
	for(int i = 0; i < m_physicsComponentList.m_size; i++)
	{
		Handle curPhysicsComponentHandle = m_physicsComponentList[i];
		if (curPhysicsComponentHandle.isValid())
		{
			PhysicsComponent* curComp = curPhysicsComponentHandle.getObject<PhysicsComponent>();
			if (curComp->m_isStatic)
			{
				continue;
			}
			curComp->applyGravity();
			for (int j = 0; j < m_physicsComponentList.m_size; j++)
			{
				if (i != j && m_physicsComponentList[j].isValid())
				{
					Handle otherPhysicsComponentHandle = m_physicsComponentList[j];
					PhysicsComponent* otherComponent = otherPhysicsComponentHandle.getObject<PhysicsComponent>();
					
					Vector3 collidedVec = curComp->m_collider->isCollided(curComp->m_target,  otherComponent->m_collider);
					if (collidedVec.length() != 0.f)
					{
						curComp->redirect(collidedVec);
						OutputDebugStringA("PE: PROGRESS: PhysicsManager::do_PHYSICS_UPDATE() Received: collided: ");
					}
				}
			}
		}
	}
		
}

void PhysicsManager::do_POST_PHYSICS_UPDATE(Events::Event* pEvt)
{
	for(int i = 0; i < m_physicsComponentList.m_size; i++)
	{
		Handle curPhysicsComponentHandle = m_physicsComponentList[i];
		if (curPhysicsComponentHandle.isValid())
		{
			PhysicsComponent* curComp = curPhysicsComponentHandle.getObject<PhysicsComponent>();
			if (curComp->m_isStatic)
			{
				continue;
			}
			Handle parentSceneNodeHandle = curComp->getFirstParentByType<SceneNode>();
			SceneNode *pSN = parentSceneNodeHandle.getObject<SceneNode>();
			Handle hMainSN = pSN->getFirstParentByType<SceneNode>();
			SceneNode* mainSN = hMainSN.getObject<SceneNode>();

			Handle colliderHandle = curComp->getFirstComponentHandle<Collider>();
			Collider* curCollider = colliderHandle.getObject<Collider>();

			Vector3 targetPos = curComp->m_target;
			mainSN->m_base.setPos(targetPos);
			Vector3 newPos = mainSN->m_base.getPos();
			curCollider->updatePos(mainSN->m_base);
		}
	}

}
}; // namespace Components
}; //namespace PE
