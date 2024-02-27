#ifndef __PYENGINE_2_0_COLLIDER_H__
#define __PYENGINE_2_0_COLLIDER_H__

#define NOMINMAX
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes

// Inter-Engine includes
#include "../Utils/Array/Array.h"
#include <PrimeEngine/Math/Vector3.h>

#include "CameraSceneNode.h"
#include "SceneNode.h"
#include "Mesh.h"

namespace PE
{
    namespace Components
    {
        enum ColliderShape
        {
            Sphere,
            Box,
            // Capsule
        };
        struct Collider : public Component
        {
            PE_DECLARE_CLASS(Collider);

            // Constructor -------------------------------------------------------------
            Collider(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself);
            // virtual void addDefaultComponents();

            ColliderShape m_shape;
            Matrix4x4 m_center;
            Matrix4x4 m_worldTransform;
            virtual Vector3 isCollided(Vector3 m_target, Collider* other) = NULL;
            virtual void updatePos(Matrix4x4 m) = 0;
        };
        struct SphereCollider: Collider
        {
            SphereCollider(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself, SceneNode* parent, float radius) :Collider(context, arena, hMyself)
            {
                m_center.loadIdentity();
                m_center.moveUp(radius);
                m_worldTransform = parent->m_worldTransform * m_center;
                m_radius = radius;
            }

            float m_radius;
            Vector3 isCollided(Vector3 m_target, Collider* other);
            void updatePos(Matrix4x4 m);
        };

        struct BoxCollider: Collider
        {
            Plane m_planes[6];
            float halfZ;
            float halfY;
            float halfX;
            BoxCollider(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself, SceneNode* m_parent, Mesh::BoundingBox m_aabb):Collider(context, arena, hMyself)
            {
                halfZ = (m_aabb.max_Z - m_aabb.min_Z) / 2;
                halfY = (m_aabb.max_Y - m_aabb.min_Y) / 2;
                halfX = (m_aabb.max_X - m_aabb.min_X) / 2;

                m_center.loadIdentity();
                m_center.moveUp(halfY);
                m_worldTransform = m_parent->m_base * m_center;

                Matrix4x4 leftPoint = Matrix4x4(m_worldTransform);
                leftPoint.moveLeft(halfX);
                m_planes[0] = {-m_parent->m_base.getU(), leftPoint.getPos() * (-m_parent->m_base.getU()) };


                Matrix4x4 rightPoint = Matrix4x4(m_worldTransform);
                rightPoint.moveRight(halfX);
                m_planes[1] = {m_parent->m_base.getU(), rightPoint.getPos() * (m_parent->m_base.getU()) };


                Matrix4x4 topPoint = Matrix4x4(m_worldTransform);
                topPoint.moveUp(halfY);
                m_planes[2] = {m_parent->m_base.getV(), topPoint.getPos() * (m_parent->m_base.getV()) };


                Matrix4x4 bottomPoint = Matrix4x4(m_worldTransform);
                bottomPoint.moveDown(halfY);
                m_planes[3] = {-m_parent->m_base.getV(), bottomPoint.getPos() * (-m_parent->m_base.getV()) };


                Matrix4x4 frontPoint = Matrix4x4(m_worldTransform);
                frontPoint.moveBack(halfZ);
                m_planes[4] = {-m_parent->m_base.getN(), frontPoint.getPos() * (-m_parent->m_base.getN()) };


                Matrix4x4 backPoint = Matrix4x4(m_worldTransform);
                backPoint.moveForward(halfZ);
                m_planes[5] = {m_parent->m_base.getN(), backPoint.getPos() * (m_parent->m_base.getN()) };
            }

            Vector3 isCollided(Vector3 m_target, Collider* other);
            void updatePos(Matrix4x4 m);
        };
    }
};
#endif