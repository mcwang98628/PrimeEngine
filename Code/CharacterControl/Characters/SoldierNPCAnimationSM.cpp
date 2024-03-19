#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

#include "PrimeEngine/Lua/LuaEnvironment.h"

#include "SoldierNPCAnimationSM.h"
#include "SoldierNPC.h"

using namespace PE::Components;
using namespace PE::Events;

namespace CharacterControl{

namespace Events{
PE_IMPLEMENT_CLASS1(SoldierNPCAnimSM_Event_STOP, Event);

PE_IMPLEMENT_CLASS1(SoldierNPCAnimSM_Event_WALK, Event);

PE_IMPLEMENT_CLASS1(SoldierNPCAnimSM_Event_RUN, Event);

PE_IMPLEMENT_CLASS1(SoldierNPCAnimSM_Event_STAND_AND_SHOOT, Event);
	
PE_IMPLEMENT_CLASS1(SoldierNPCAnimSM_Event_SAMPLE_BLEND, Event);
PE_IMPLEMENT_CLASS1(SoldierNPCAnimSM_Event_SAMPLE_PARTIAL, Event);
PE_IMPLEMENT_CLASS1(SoldierNPCAnimSM_Event_SAMPLE_ADDITIVE, Event);

}
namespace Components{

PE_IMPLEMENT_CLASS1(SoldierNPCAnimationSM, DefaultAnimationSM);

SoldierNPCAnimationSM::SoldierNPCAnimationSM(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself) : DefaultAnimationSM(context, arena, hMyself)
{
	m_curId = NONE;
}

void SoldierNPCAnimationSM::addDefaultComponents()
{
	DefaultAnimationSM::addDefaultComponents();
	PE_REGISTER_EVENT_HANDLER(Events::SoldierNPCAnimSM_Event_STOP, SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_STOP);
	PE_REGISTER_EVENT_HANDLER(Events::SoldierNPCAnimSM_Event_WALK, SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_WALK);
	PE_REGISTER_EVENT_HANDLER(Events::SoldierNPCAnimSM_Event_RUN, SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_RUN);
	PE_REGISTER_EVENT_HANDLER(Events::SoldierNPCAnimSM_Event_STAND_AND_SHOOT, SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_STAND_AND_SHOOT);
	
	PE_REGISTER_EVENT_HANDLER(Events::SoldierNPCAnimSM_Event_SAMPLE_BLEND, SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_SAMPLE_BLEND);
	PE_REGISTER_EVENT_HANDLER(Events::SoldierNPCAnimSM_Event_SAMPLE_PARTIAL, SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_SAMPLE_PARTIAL);
	PE_REGISTER_EVENT_HANDLER(Events::SoldierNPCAnimSM_Event_SAMPLE_ADDITIVE, SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_SAMPLE_ADDITIVE);

}

void SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_STOP(PE::Events::Event *pEvt)
{
	
	if (m_curId != SoldierNPCAnimationSM::STAND)
	{
		m_curId = SoldierNPCAnimationSM::STAND;
		
		setAnimation(0, SoldierNPCAnimationSM::STAND,
		0, 1, 0, 0,
		PE::LOOPING);
	}
}

void SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_WALK(PE::Events::Event *pEvt)
{
	// if (m_curId != SoldierNPCAnimationSM::WALK)
	// {
	// 	m_curId = SoldierNPCAnimationSM::WALK;
	// 	setAnimation(0, SoldierNPCAnimationSM::WALK,
	// 		0, 0, 1, 1,
	// 		PE::LOOPING);
	// }
}

void SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_RUN(PE::Events::Event *pEvt)
{
	if (m_curId != SoldierNPCAnimationSM::RUN)
	{
		m_curId = SoldierNPCAnimationSM::RUN;
		setAnimation(0, SoldierNPCAnimationSM::RUN,
			0, 1, 0, 0,
			PE::LOOPING);
	}
}

void SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_STAND_AND_SHOOT(PE::Events::Event *pEvt)
{
	if (m_curId != SoldierNPCAnimationSM::STAND_SHOOT)
	{
		m_curId = SoldierNPCAnimationSM::STAND_SHOOT;
		setAnimation(0, SoldierNPCAnimationSM::STAND_SHOOT,
			0, 1, 0, 0,
			PE::LOOPING);
	}
}

void SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_SAMPLE_BLEND(PE::Events::Event *pEvt)
 {
	if (m_curId != STAND)
	{
		m_curId = STAND;
		setAnimation(1, SoldierNPCAnimationSM::STAND,
0, 0, 1, 1,
PE::LOOPING);
		setAnimation(1, SoldierNPCAnimationSM::STAND,
		1, 1, 0, 0,
		PE::LOOPING);
	}
	else if (m_curId == STAND)
	{
		m_curId = RUN;
		setAnimation(1, SoldierNPCAnimationSM::RUN,
					1, 1, 0, 0,
					PE::LOOPING);
	}
 }

void SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_SAMPLE_PARTIAL(PE::Events::Event *pEvt)
{
	if (m_curId != SoldierNPCAnimationSM::STAND && m_curId != SoldierNPCAnimationSM::RUN)
	{
		m_curId = SoldierNPCAnimationSM::STAND;
		
		setAnimation(1, SoldierNPCAnimationSM::STAND,
		0, 0, 1, 1,
		PE::LOOPING);
		setAnimation(1, SoldierNPCAnimationSM::STAND,
		1, 1, 0, 0,
		PE::LOOPING);
	}
	else if (m_curId == STAND)
	{
		m_curId = RUN;
		setAnimation(1, SoldierNPCAnimationSM::RUN,
					0, 1, 0, 0,
					PE::LOOPING);
	}
	else if (m_curId == RUN)
	{
		m_curId = STAND_SHOOT;
		setAnimation(1, SoldierNPCAnimationSM::STAND_SHOOT,
					0, 1, 0, 0,
					PE::LOOPING);
	}
}
void SoldierNPCAnimationSM::do_SoldierNPCAnimSM_Event_SAMPLE_ADDITIVE(PE::Events::Event *pEvt)
{
	if (m_additiveAnim == NULL)
	{
		generateAdditiveSource(1,2, 0 ,3, 1, 1);
	}
	if (m_curId != SoldierNPCAnimationSM::STAND && m_curId != SoldierNPCAnimationSM::RUN && m_curId != SoldierNPCAnimationSM::STAND_SHOOT)
	{
		m_curId = SoldierNPCAnimationSM::STAND;
		
		setAnimation(1, SoldierNPCAnimationSM::STAND,
		0, 0, 1, 1,
		PE::LOOPING);
		setAnimation(1, SoldierNPCAnimationSM::STAND,
		1, 1, 0, 0,
		PE::LOOPING);
	}
	else if (m_curId == STAND)
	{
		m_curId = RUN;
		setAnimation(1, SoldierNPCAnimationSM::RUN,
					0, 1, 0, 0,
					PE::LOOPING);
	}
	else if (m_curId == RUN)
	{
		m_curId = STAND_SHOOT;
		setAnimation(1, SoldierNPCAnimationSM::STAND_SHOOT,
					0, 0, 2, 2,
					PE::LOOPING);
	}
	else if (m_curId == STAND_SHOOT)
	{
		m_curId = ADDITIVE_AIM;
		setAnimation(1, SoldierNPCAnimationSM::ADDITIVE_AIM,
					0, 0, 1, 1,
					PE::LOOPING);
	}
	
	
}
}}




