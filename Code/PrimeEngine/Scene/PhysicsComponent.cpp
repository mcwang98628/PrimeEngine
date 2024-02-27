#define NOMINMAX
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes

// Inter-Engine includes
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"

// Sibling/Children includes

#include "PhysicsComponent.h"

#include "../Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"
#include "PhysicsComponent.h"

namespace PE {
namespace Components {

PE_IMPLEMENT_CLASS1(PhysicsComponent, Component);

PhysicsComponent::PhysicsComponent(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself, bool isStatic)
	: Component(context, arena, hMyself), m_gravity(-0.1f)
{
	m_isStatic = isStatic;
	m_collider = NULL;
}

void PhysicsComponent::createBoxCollider(SceneNode* parentSceneNode, Mesh::BoundingBox m_aabb)
{
    Handle hCollider("BoxCollider", sizeof(BoxCollider));
	m_collider = new(hCollider) BoxCollider(*m_pContext, m_arena, hCollider, parentSceneNode, m_aabb);
	m_collider->m_shape = Box;
	m_collider->addDefaultComponents();
	addComponent(m_collider);
}

void PhysicsComponent::createSphereCollider(SceneNode* parentSceneNode, float radius)
{
    Handle hCollider("SphereCollider", sizeof(SphereCollider));
	m_collider = new(hCollider) SphereCollider(*m_pContext, m_arena, hCollider, parentSceneNode, radius);
	m_collider->m_shape = Sphere;
	m_collider->addDefaultComponents();
	addComponent(m_collider);
}
	
void PhysicsComponent::applyGravity()
{
	m_target += Vector3(0.f, m_gravity, 0.f);
}

void PhysicsComponent::redirect(Vector3 newVec)
{
	m_target -= newVec;
}
	
}}