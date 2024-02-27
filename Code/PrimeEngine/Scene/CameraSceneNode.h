#ifndef __PYENGINE_2_0_CAMERA_SCENE_NODE_H__
#define __PYENGINE_2_0_CAMERA_SCENE_NODE_H__

// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "SceneNode.h"
#include "PrimeEngine/Render/IRenderer.h"
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "../Events/Component.h"
#include "../Utils/Array/Array.h"
#include "PrimeEngine/Math/CameraOps.h"

// Sibling/Children includes

namespace PE
{
    namespace Components
    {
        struct Plane {
            Vector3 m_normal;
            float distance;

            bool checkIfOnOrAbove(Vector3 position) 
            {
                float pointVec = m_normal * position;
                float distanceFromPlane = pointVec - distance;
                return distanceFromPlane >= 0;
            }

            float distanceFromPlane(Vector3 position) 
            {
                float pointVec = position * m_normal;
                float distanceFromPlane = pointVec - distance;
                return distanceFromPlane;
            }

	
        };
        struct Frustum
        {
            Plane nearPlane;
            Plane farPlane;

            Plane leftPlane;
            Plane rightPlane;

            Plane topPlane;
            Plane bottomPlane;

            Vector3 nearLeftTop;
            Vector3 nearLeftBot;

            Vector3 nearRightTop;
            Vector3 nearRightBot;

            Vector3 farRightTop;
            Vector3 farRightBot;

            Vector3 farLeftTop;
            Vector3 farLeftBot;

            bool isInFrustum(Vector3 position) 
            {
                bool isAboveNear = nearPlane.checkIfOnOrAbove(position);
                bool isAboveFar = farPlane.checkIfOnOrAbove(position);
                bool isAboveRight = nearPlane.checkIfOnOrAbove(position);
                bool isAboveLeft = nearPlane.checkIfOnOrAbove(position);
                bool isAboveBot = nearPlane.checkIfOnOrAbove(position);
                bool isAboveTop = nearPlane.checkIfOnOrAbove(position);

                // return true;
                // return nearPlane.checkIfOnOrAbove(position) &&
                // 	farPlane.checkIfOnOrAbove(position);
                return nearPlane.checkIfOnOrAbove(position) &&
                    farPlane.checkIfOnOrAbove(position) &&
                    leftPlane.checkIfOnOrAbove(position) &&
                    rightPlane.checkIfOnOrAbove(position) &&
                    topPlane.checkIfOnOrAbove(position) &&
                    bottomPlane.checkIfOnOrAbove(position);
            }
        };
        struct CameraSceneNode : public SceneNode
        {
            PE_DECLARE_CLASS(CameraSceneNode);

            // Constructor -------------------------------------------------------------
            CameraSceneNode(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself);

            virtual ~CameraSceneNode(){}

            // Component ------------------------------------------------------------
            virtual void addDefaultComponents();

            PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_CALCULATE_TRANSFORMATIONS);
            virtual void do_CALCULATE_TRANSFORMATIONS(Events::Event *pEvt);

            // Individual events -------------------------------------------------------
	
            Matrix4x4 m_worldToViewTransform; // objects in world space are multiplied by this to get them into camera's coordinate system (view space)
            Matrix4x4 m_worldToViewTransform2;
            Matrix4x4 m_worldTransform2;
            Matrix4x4 m_viewToProjectedTransform; // objects in local (view) space are multiplied by this to get them to screen space
            float m_near, m_far;
            Frustum m_frustum;
        };
    }; // namespace Components
}; // namespace PE
#endif
