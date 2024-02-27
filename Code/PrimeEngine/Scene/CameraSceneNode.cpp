#include "CameraSceneNode.h"
#include "../Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"
#include "DebugRenderer.h"

#define Z_ONLY_CAM_BIAS 0.0f
namespace PE {
namespace Components {

PE_IMPLEMENT_CLASS1(CameraSceneNode, SceneNode);

CameraSceneNode::CameraSceneNode(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself) : SceneNode(context, arena, hMyself)
{
	m_near = .05f;
	m_far = 2000.f;
}
void CameraSceneNode::addDefaultComponents()
{
	Component::addDefaultComponents();
	PE_REGISTER_EVENT_HANDLER(Events::Event_CALCULATE_TRANSFORMATIONS, CameraSceneNode::do_CALCULATE_TRANSFORMATIONS);
}

void CameraSceneNode::do_CALCULATE_TRANSFORMATIONS(Events::Event *pEvt)
{
	Handle hParentSN = getFirstParentByType<SceneNode>();
	if (hParentSN.isValid())
	{
		Matrix4x4 parentTransform = hParentSN.getObject<PE::Components::SceneNode>()->m_worldTransform;
		m_worldTransform = parentTransform * m_base;
	}
	
	Matrix4x4 &mref_worldTransform = m_worldTransform;

	Vector3 pos = Vector3(mref_worldTransform.m[0][3], mref_worldTransform.m[1][3], mref_worldTransform.m[2][3]);
	Vector3 n = Vector3(mref_worldTransform.m[0][2], mref_worldTransform.m[1][2], mref_worldTransform.m[2][2]);
	Vector3 target = pos + n;
	Vector3 up = Vector3(mref_worldTransform.m[0][1], mref_worldTransform.m[1][1], mref_worldTransform.m[2][1]);
	Vector3 right = up.crossProduct(n);

	m_worldToViewTransform = CameraOps::CreateViewMatrix(pos, target, up);

	m_worldTransform2 = mref_worldTransform;

	m_worldTransform2.moveForward(Z_ONLY_CAM_BIAS);

	Vector3 pos2 = Vector3(m_worldTransform2.m[0][3], m_worldTransform2.m[1][3], m_worldTransform2.m[2][3]);
	Vector3 n2 = Vector3(m_worldTransform2.m[0][2], m_worldTransform2.m[1][2], m_worldTransform2.m[2][2]);
	Vector3 target2 = pos2 + n2;
	Vector3 up2 = Vector3(m_worldTransform2.m[0][1], m_worldTransform2.m[1][1], m_worldTransform2.m[2][1]);
	Vector3 right2 = up2.crossProduct(n);

	m_worldToViewTransform2 = CameraOps::CreateViewMatrix(pos2, target2, up2);
    
    PrimitiveTypes::Float32 aspect = (PrimitiveTypes::Float32)(m_pContext->getGPUScreen()->getWidth()) / (PrimitiveTypes::Float32)(m_pContext->getGPUScreen()->getHeight());
    
    PrimitiveTypes::Float32 verticalFov = 0.34f * PrimitiveTypes::Constants::c_Pi_F32;


    if (aspect < 1.0f)
    {
        //ios portrait view
        static PrimitiveTypes::Float32 factor = 0.5f;
        verticalFov *= factor;
    }
	Vector3 moreTarget = pos + 10 * n;
	Vector3 tartar = moreTarget + 100.f * up;

	// Vector3 linepts[] = { moreTarget, Vector3(0.f, 1.f, 1.f), tartar, Vector3(0.f, 1.f, 1.f) };
	// DebugRenderer::Instance()->createLineMesh(false, Matrix4x4(), &linepts[0].m_x, 2, 0);

	m_viewToProjectedTransform = CameraOps::CreateProjectionMatrix(verticalFov, 
		aspect,
		m_near, m_far);

	// float m_near2 = 10.f;
	// float m_far2 = 20.f;
	// PrimitiveTypes::Float32 verticalFov2 = 0.3f * PrimitiveTypes::Constants::c_Pi_F32;


	const float halfVSide = m_far * tanf(verticalFov * .5f);
	const float halfHSide = halfVSide * aspect;

	const float halfNearVSide = m_near * tanf(verticalFov * .5f);
	const float halfNearHSide = halfNearVSide * aspect;

	const Vector3 frontMultFar = m_far * n;

	n.normalize();
	up.normalize();
	right.normalize();

	m_frustum.nearPlane = { n, ((pos + (n * m_near)) * n) };
	m_frustum.farPlane = { -n, (pos + frontMultFar) * (-n) };

	Vector3 rightNormal = (frontMultFar + right * halfHSide).crossProduct(up);
	rightNormal.normalize();
	Vector3 leftNormal = (up).crossProduct(frontMultFar - right * halfHSide);
	leftNormal.normalize();

	m_frustum.rightPlane = { rightNormal, (pos * rightNormal) };
	m_frustum.leftPlane = { leftNormal , (pos * leftNormal) };

	Vector3 topNormal = right.crossProduct(frontMultFar + up * halfVSide);
	topNormal.normalize();
	Vector3 bottomNormal = (frontMultFar - up * halfVSide).crossProduct(right);
	bottomNormal.normalize();

	m_frustum.topPlane = { topNormal, (pos * topNormal)};
	m_frustum.bottomPlane = { bottomNormal, (pos * bottomNormal)};

	m_frustum.nearLeftBot = (pos + (n * m_near)) - right *  halfNearHSide - up * halfNearVSide;
	m_frustum.nearLeftTop = (pos + (n * m_near)) - right *  halfNearHSide + up * halfNearVSide;
	m_frustum.nearRightBot = (pos + (n * m_near)) + right *  halfNearHSide - up * halfNearVSide;
	m_frustum.nearRightTop = (pos + (n * m_near)) + right *  halfNearHSide + up * halfNearVSide;

	m_frustum.farLeftBot = (pos + (n * m_far)) - right *  halfHSide - up * halfVSide;
	m_frustum.farLeftTop = (pos + (n * m_far)) - right *  halfHSide + up * halfVSide;
	m_frustum.farRightBot = (pos + (n * m_far)) + right *  halfHSide - up * halfVSide;
	m_frustum.farRightTop = (pos + (n * m_far)) + right *  halfHSide + up * halfVSide;


	SceneNode::do_CALCULATE_TRANSFORMATIONS(pEvt);

}

}; // namespace Components
}; // namespace PE
