#ifndef __PYENGINE_2_0_PHYSICS_COMPONENT_H__
#define __PYENGINE_2_0_PHYSICS_COMPONENT_H__

#define NOMINMAX
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes

// Inter-Engine includes
#include "../Utils/Array/Array.h"
#include "../Utils/PEString.h"
#include "Collider.h"
#include "Mesh.h"
#include "SceneNode.h"

namespace PE {
namespace Components {

struct PhysicsComponent : public Component
{
    PE_DECLARE_CLASS(PhysicsComponent);

    // Constructor -------------------------------------------------------------
    PhysicsComponent(PE::GameContext& context, PE::MemoryArena arena, Handle hMyself, bool isStatic);

    virtual ~PhysicsComponent() {}

    // virtual void addDefaultComponents();
    void createBoxCollider(SceneNode* parentSceneNode, Mesh::BoundingBox m_aabb);

    virtual void createSphereCollider(SceneNode* parentSceneNode, float radius);
    
    void redirect(Vector3 newVec);
    
    void applyGravity();

    Vector3 m_target;

    Collider* m_collider;
    bool m_isStatic;
    const float m_gravity;

};
}; // namespace Components
}; // namespace PE

#endif
