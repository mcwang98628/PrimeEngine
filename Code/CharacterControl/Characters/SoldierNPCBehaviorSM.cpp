#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Scene/DebugRenderer.h"
#include "../ClientGameObjectManagerAddon.h"
#include "../CharacterControlContext.h"
#include "SoldierNPCMovementSM.h"
#include "SoldierNPCAnimationSM.h"
#include "SoldierNPCBehaviorSM.h"
#include "SoldierNPC.h"
#include "PrimeEngine/Scene/SceneNode.h"
#include "PrimeEngine/Render/IRenderer.h"
#include "PrimeEngine/Scene/MeshInstance.h"
#include "PrimeEngine/Scene/SkeletonInstance.h"

using namespace PE::Components;
using namespace PE::Events;
using namespace CharacterControl::Events;

namespace CharacterControl{

namespace Components{

PE_IMPLEMENT_CLASS1(SoldierNPCBehaviorSM, Component);

SoldierNPCBehaviorSM::SoldierNPCBehaviorSM(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself, PE::Handle hMovementSM) 
: Component(context, arena, hMyself)
, m_hMovementSM(hMovementSM)
{

}

void SoldierNPCBehaviorSM::start()
{
	if (m_havePatrolWayPoint)
	{
		m_state = WAITING_FOR_WAYPOINT; // will update on next do_UPDATE()
	}
	else
	{
		m_state = IDLE; // stand in place

		PE::Handle h("SoldierNPCMovementSM_Event_STOP", sizeof(SoldierNPCMovementSM_Event_STOP));
		SoldierNPCMovementSM_Event_STOP *pEvt = new(h) SoldierNPCMovementSM_Event_STOP();

		m_hMovementSM.getObject<Component>()->handleEvent(pEvt);
		// release memory now that event is processed
		h.release();
	}	
}


void SoldierNPCBehaviorSM::addDefaultComponents()
{
	Component::addDefaultComponents();

	PE_REGISTER_EVENT_HANDLER(SoldierNPCMovementSM_Event_TARGET_REACHED, SoldierNPCBehaviorSM::do_SoldierNPCMovementSM_Event_TARGET_REACHED);
	PE_REGISTER_EVENT_HANDLER(Event_UPDATE, SoldierNPCBehaviorSM::do_UPDATE);

	PE_REGISTER_EVENT_HANDLER(Event_PRE_RENDER_needsRC, SoldierNPCBehaviorSM::do_PRE_RENDER_needsRC);
}

void SoldierNPCBehaviorSM::do_SoldierNPCMovementSM_Event_TARGET_REACHED(PE::Events::Event *pEvt)
{
	PEINFO("SoldierNPCBehaviorSM::do_SoldierNPCMovementSM_Event_TARGET_REACHED\n");

	if (m_state == PATROLLING_WAYPOINTS)
	{
		ClientGameObjectManagerAddon *pGameObjectManagerAddon = (ClientGameObjectManagerAddon *)(m_pContext->get<CharacterControlContext>()->getGameObjectManagerAddon());
		if (pGameObjectManagerAddon)
		{
			// search for waypoint object
			WayPoint *pWP = pGameObjectManagerAddon->getWayPoint(m_curPatrolWayPoint);

			char* curWayPoint;
			bool wayPointFlag = rand() % 2 > 0;
			curWayPoint = wayPointFlag ? pWP->m_nextChoice1 : pWP->m_nextChoice2;

			if (pWP && StringOps::length(curWayPoint) > 0 && m_curPatrolWayPoint != curWayPoint)
			{
				// have next waypoint to go to
				pWP = pGameObjectManagerAddon->getWayPoint(curWayPoint);
				if (pWP)
				{
					StringOps::writeToString(pWP->m_name, m_curPatrolWayPoint, 32);

					m_state = PATROLLING_WAYPOINTS;
					PE::Handle h("SoldierNPCMovementSM_Event_MOVE_TO", sizeof(SoldierNPCMovementSM_Event_MOVE_TO));
					Events::SoldierNPCMovementSM_Event_MOVE_TO *pEvt = new(h) SoldierNPCMovementSM_Event_MOVE_TO(pWP->m_base.getPos());
					pEvt->m_running = true;

					m_hMovementSM.getObject<Component>()->handleEvent(pEvt);
					// release memory now that event is processed
					h.release();
				}
			}
			else
			{
				m_state = IDLE;
				// no need to send the event. movement state machine will automatically send event to animation state machine to play idle animation
			}
		}
	}
}

// this event is executed when thread has RC
void SoldierNPCBehaviorSM::do_PRE_RENDER_needsRC(PE::Events::Event *pEvt)
{
	Event_PRE_RENDER_needsRC *pRealEvent = (Event_PRE_RENDER_needsRC *)(pEvt);
	if (m_havePatrolWayPoint)
	{
		// char buf[80];
		// sprintf(buf, "Patrol Waypoint: %s",m_curPatrolWayPoint);
		SoldierNPC *pSol = getFirstParentByTypePtr<SoldierNPC>();
		PE::Handle hSoldierSceneNode = pSol->getFirstComponentHandle<PE::Components::SceneNode>();
		Matrix4x4 base = hSoldierSceneNode.getObject<PE::Components::SceneNode>()->m_worldTransform;
		
		// DebugRenderer::Instance()->createTextMesh(
		// 	buf, false, false, true, false, 0,
		// 	base.getPos(), 0.01f, pRealEvent->m_threadOwnershipMask);
		
		{
			//we can also construct points ourself
			bool sent = false;
			ClientGameObjectManagerAddon *pGameObjectManagerAddon = (ClientGameObjectManagerAddon *)(m_pContext->get<CharacterControlContext>()->getGameObjectManagerAddon());
			if (pGameObjectManagerAddon)
			{
				WayPoint *pWP = pGameObjectManagerAddon->getWayPoint(m_curPatrolWayPoint);
				if (pWP)
				{
					Vector3 target = pWP->m_base.getPos();
					Vector3 pos = base.getPos();
					Vector3 color(1.0f, 1.0f, 0);
					Vector3 linepts[] = {pos, color, target, color};
					
					DebugRenderer::Instance()->createLineMesh(true, base,  &linepts[0].m_x, 2, 0);// send event while the array is on the stack
					sent = true;
				}
			}
			if (!sent) // if for whatever reason we didnt retrieve waypoint info, send the event with transform only
				DebugRenderer::Instance()->createLineMesh(true, base, NULL, 0, 0);// send event while the array is on the stack
		}
	}
}

void SoldierNPCBehaviorSM::do_UPDATE(PE::Events::Event *pEvt)
{
	if (m_state == WAITING_FOR_WAYPOINT)
	{
		if (m_havePatrolWayPoint)
		{
			ClientGameObjectManagerAddon *pGameObjectManagerAddon = (ClientGameObjectManagerAddon *)(m_pContext->get<CharacterControlContext>()->getGameObjectManagerAddon());
			if (pGameObjectManagerAddon)
			{
				// search for waypoint object
				WayPoint *pWP = pGameObjectManagerAddon->getWayPoint(m_curPatrolWayPoint);
				if (pWP)
				{
					m_state = PATROLLING_WAYPOINTS;
					PE::Handle h("SoldierNPCMovementSM_Event_MOVE_TO", sizeof(SoldierNPCMovementSM_Event_MOVE_TO));
					Events::SoldierNPCMovementSM_Event_MOVE_TO *pEvt = new(h) SoldierNPCMovementSM_Event_MOVE_TO(pWP->m_base.getPos());

					m_hMovementSM.getObject<Component>()->handleEvent(pEvt);
					// release memory now that event is processed
					h.release();
				}
			}
		}
		else
		{
			// should not happen, but in any case, set state to idle
			m_state = IDLE;

			PE::Handle h("SoldierNPCMovementSM_Event_STOP", sizeof(SoldierNPCMovementSM_Event_STOP));
			SoldierNPCMovementSM_Event_STOP *pEvt = new(h) SoldierNPCMovementSM_Event_STOP();

			m_hMovementSM.getObject<Component>()->handleEvent(pEvt);
			// release memory now that event is processed
			h.release();
		}
	}

	if (m_state == IDLE) 
	{
		ClientGameObjectManagerAddon *pGameObjectManagerAddon = (ClientGameObjectManagerAddon *)(m_pContext->get<CharacterControlContext>()->getGameObjectManagerAddon());
		if (pGameObjectManagerAddon)
		{
			SoldierNPC *pS = pGameObjectManagerAddon->getEnemy(m_curEnemy);
			if (pS) 
			{
				PE::Handle hSoldierSceneNodeHandler = pS->getFirstComponentHandle<PE::Components::SceneNode>();
				SceneNode* enemySceneNode = hSoldierSceneNodeHandler.getObject<PE::Components::SceneNode>();
				Matrix4x4 enemyBase = enemySceneNode->m_worldTransform;
				Vector3 enemyPos = enemyBase.getPos();

				SoldierNPC *pSol = getFirstParentByTypePtr<SoldierNPC>();
				PE::Handle curSoldierSceneNode = pSol->getFirstComponentHandle<PE::Components::SceneNode>();
				Matrix4x4 base = curSoldierSceneNode.getObject<PE::Components::SceneNode>()->m_worldTransform;
				Vector3 pos = base.getPos();

				float distance = sqrt((pos.m_x - enemyPos.m_x) * (pos.m_x - enemyPos.m_x) + (pos.m_y - enemyPos.m_y) * (pos.m_y - enemyPos.m_y) + (pos.m_z - enemyPos.m_z) * (pos.m_z - enemyPos.m_z));
				// PEINFO("SoldierNPCBehaviorSM::do_UPDATE distance is %s \n", distance);

				if (distance < 6.f) {
					// m_state = COMBAT;
				
					PE::Handle h("SoldierNPCMovementSM_Event_ENEMY_DETECTED", sizeof(SoldierNPCMovementSM_Event_ENEMY_DETECTED));
					SoldierNPCMovementSM_Event_ENEMY_DETECTED *pEvt = new(h) SoldierNPCMovementSM_Event_ENEMY_DETECTED(enemySceneNode);
				
					m_hMovementSM.getObject<Component>()->handleEvent(pEvt);
					// release memory now that event is processed
					h.release();
				}
			}
		}
	}
	if(m_state == SAMPLE_ANIMATION)
	{
		Event_UPDATE* pRealEvt = (Event_UPDATE*)(pEvt);
		m_pastTime += pRealEvt->m_frameTime;
		if (m_pastTime >= m_exampleTimeThreshold)
		{
			startAnimationSample();
		}
	}
	// if (m_state == COMBAT) {
		// ClientGameObjectManagerAddon *pGameObjectManagerAddon = (ClientGameObjectManagerAddon *)(m_pContext->get<CharacterControlContext>()->getGameObjectManagerAddon());
		// if (pGameObjectManagerAddon)
		// {
		// 	SoldierNPC *pS = pGameObjectManagerAddon->getEnemy(m_curEnemy);
		// 	if (pS) 
		// 	{
		// 		PE::Handle hSoldierSceneNodeHandler = pS->getFirstComponentHandle<PE::Components::SceneNode>();
		// 		SceneNode* enemySceneNode = hSoldierSceneNodeHandler.getObject<PE::Components::SceneNode>();
		// 		Matrix4x4 enemyBase = enemySceneNode->m_worldTransform;
		// 		Vector3 enemyPos = enemyBase.getPos();

		// 		PE::Handle h("SoldierNPCMovementSM_Event_ENEMY_DETECTED", sizeof(SoldierNPCMovementSM_Event_ENEMY_DETECTED));
		// 		SoldierNPCMovementSM_Event_ENEMY_DETECTED *pEvt = new(h) SoldierNPCMovementSM_Event_ENEMY_DETECTED(enemySceneNode);
		// 		OutputDebugStringA("PE: PROGRESS: SoldierNPCBehaviorSM::SoldierNPCMovementSM_Event_ENEMY_DETECTED() during combat \n ");


		// 		m_hMovementSM.getObject<Component>()->handleEvent(pEvt);
		// 		// release memory now that event is processed
		// 		h.release();
		// 	}
		// }
	// }
}

void SoldierNPCBehaviorSM::startAnimationSample()
{
	m_state = SAMPLE_ANIMATION;
	SoldierNPC *pSol = getFirstParentByTypePtr<SoldierNPC>();
	m_pastTime = 0;
	if (StringOps::strcmp(pSol->m_animationType, "blendAnim") == 0)
	{
		Events::SoldierNPCAnimSM_Event_SAMPLE_BLEND Evt;
		pSol->getFirstComponent<PE::Components::SceneNode>()->handleEvent(&Evt);
	}
	else if (StringOps::strcmp(pSol->m_animationType, "partial") == 0)
	{
		Events::SoldierNPCAnimSM_Event_SAMPLE_PARTIAL Evt;
		pSol->getFirstComponent<PE::Components::SceneNode>()->handleEvent(&Evt);
	}
	else if (StringOps::strcmp(pSol->m_animationType, "additive") == 0)
	{
		Events::SoldierNPCAnimSM_Event_SAMPLE_ADDITIVE Evt;
		pSol->getFirstComponent<PE::Components::SceneNode>()->handleEvent(&Evt);
	}
}
}}