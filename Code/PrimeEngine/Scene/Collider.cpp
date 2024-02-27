#define NOMINMAX
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes

// Inter-Engine includes
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"

// Sibling/Children includes

#include "Collider.h"

#include "../Lua/LuaEnvironment.h"

#include "PhysicsComponent.h"

namespace PE {
namespace Components {

PE_IMPLEMENT_CLASS1(Collider, Component);

Collider::Collider(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself)
	: Component(context, arena, hMyself)
{

}

Vector3 BoxCollider::isCollided(Vector3 m_target, Collider* other) {
	// if (other->m_shape == Box)
	// {
	// 	BoxCollider* box = dynamic_cast<BoxCollider*>(other);
		

	// }
	// else if (other->m_shape == Sphere)
	// {
	// 	SphereCollider* sphere = dynamic_cast<SphereCollider*>(other);
	// 	float x = max(box.minX, min(sphere.x, box.maxX));
	// 	float y = max(box.minY, min(sphere.y, box.maxY));
	// 	float z = max(box.minZ, min(sphere.z, box.maxZ));
	// }
	return Vector3(0.f, 0.f, 0.f);
}
void SphereCollider::updatePos(Matrix4x4 m_pos)
{
	Matrix4x4 newTransform = Matrix4x4(m_pos);
	newTransform.moveUp(m_radius);
	m_worldTransform = newTransform;
}

void BoxCollider::updatePos(Matrix4x4 m_pos)
{
	Matrix4x4 newTransform = Matrix4x4(m_pos);
	newTransform.moveUp(halfY);
	m_worldTransform = newTransform;
}


Vector3 SphereCollider::isCollided(Vector3 m_target, Collider* other) {
	if (other->m_shape == Box)
	{
		BoxCollider* box = dynamic_cast<BoxCollider*>(other);
		float minDistance = (float)MAXINT;
		Plane closestPlane;
		for (int i = 0; i < 6; i++)
		{
			float curDistance = box->m_planes [i] .distanceFromPlane(m_worldTransform.getPos());
			if (curDistance < 0)
			{
				continue;
			}
			if (curDistance > m_radius)
			{
				return Vector3(0.f, 0.f, 0.f);
			}
			if (curDistance > 0 && curDistance < minDistance)
			{
				minDistance = curDistance;
				closestPlane = box->m_planes[i];
			}
		}
		if (minDistance <= m_radius)
		{
			Vector3 moveVec = m_target - m_worldTransform.getPos();
			// moveVec.normalize();
			// Vector3 sphereEdge = moveVec * m_radius +m_worldTransform.getPos();
			// Vector3 realMoveVec = m_target - sphereEdge;
			Vector3 nextPos = (moveVec * (closestPlane.m_normal)) * (closestPlane.m_normal);
			return nextPos;
			//
			// if (abs(nextPos.m_x) >0 ||  abs(nextPos.m_z) >0)
			// {
			// 	return nextPos;
			// }else
			// {
			// 	return nextPos;
			// }
		}
		else
		{
			return Vector3(0.f, 0.f, 0.f);
		}
	}
	else if (other->m_shape == Sphere)
	{
		SphereCollider* sphere = dynamic_cast<SphereCollider*>(other);
		Matrix4x4 otherCenter = sphere->m_worldTransform;
		Vector3 distVec = otherCenter.getPos() - m_worldTransform.getPos();
		float dist = distVec.length();
		if (dist <= m_radius + sphere->m_radius)
		{
			return Vector3(0.f, 0.f, 0.f);
		}
	}
	return Vector3(0.f, 0.f, 0.f);
}
}}
