#include "collision.h"

Collision::AABB::AABB(const XMFLOAT3& min, const XMFLOAT3& max) : m_MinSurfacePos(min), m_MaxSurfacePos(max)
{
	m_Center.x = (max.x + min.x) * 0.5f;
	m_Center.y = (max.y + min.y) * 0.5f;
	m_Center.z = (max.z + min.z) * 0.5f;
}

bool Collision::AABB::isOverlap(const AABB& target) const
{
	return m_MinSurfacePos.x < target.m_MaxSurfacePos.x
		&& m_MaxSurfacePos.x > target.m_MinSurfacePos.x
		&& m_MinSurfacePos.y < target.m_MaxSurfacePos.y
		&& m_MaxSurfacePos.y > target.m_MinSurfacePos.y
		&& m_MinSurfacePos.z < target.m_MaxSurfacePos.z
		&& m_MaxSurfacePos.z > target.m_MinSurfacePos.z;
}

bool Collision::AABB::isOverlap(const Point& target) const
{
	return m_MinSurfacePos.x < target.GetPosition().x
		&& m_MaxSurfacePos.x > target.GetPosition().x
		&& m_MinSurfacePos.y < target.GetPosition().y
		&& m_MaxSurfacePos.y > target.GetPosition().y
		&& m_MinSurfacePos.z < target.GetPosition().z
		&& m_MaxSurfacePos.z > target.GetPosition().z;
}

Collision::AABB Collision::AABB::Translation(const XMVECTOR& translate) const
{
	Collision::AABB aabb;
	XMStoreFloat3(&aabb.m_MinSurfacePos, XMLoadFloat3(&m_MinSurfacePos) + translate);
	XMStoreFloat3(&aabb.m_MaxSurfacePos, XMLoadFloat3(&m_MaxSurfacePos) + translate);
	XMStoreFloat3(&aabb.m_Center, XMLoadFloat3(&m_Center) + translate);

	return aabb;
}

Collision::Sphere Collision::Sphere::Translation(const XMVECTOR& translate) const
{
	Sphere sphere;
	XMStoreFloat3(&sphere.m_CenterPosition, XMLoadFloat3(&m_CenterPosition) + translate);

	return sphere;
}

bool Collision::Sphere::isOverlap(const Point& target) const
{
	XMVECTOR vector = XMLoadFloat3(&target.GetPosition()) - XMLoadFloat3(&m_CenterPosition);
	
	//XMVector3LengthSq(vector) < m_Radius* m_Radius;
	return false;
}

bool Collision::Sphere::isOverlap(const Sphere& target) const
{
	XMVECTOR ca = XMLoadFloat3(&m_CenterPosition);
	XMVECTOR cb = XMLoadFloat3(&target.GetPosition());
	float r = m_Radius + target.GetRadius();

	return XMVectorGetX(XMVector3LengthSq(ca - cb)) <= r * r;

	return false;
}

bool Collision::Sphere::isOverlap(const Cylinder& target) const
{
	return target.isOverlap(*this);
}

bool Collision::Sphere::isOverlap(const Capsule& target) const
{
	return target.isOverlap(*this);
}

bool Collision::Cylinder::isOverlap(const Sphere& target) const //–¢Š®¬
{
	//Å‘å“_‚ÆÅ¬“_‚ÌŽZo
	//³‹K‰»‚µ‚½ƒxƒNƒgƒ‹‚ÌŽZo
	XMVECTOR vector = XMVector3Normalize({ abs(target.GetPosition().x - m_CenterPosition.x), 0.0f, abs(target.GetPosition().z - m_CenterPosition.z)});
	//’·‚³‚ÅæŽZ
	vector *= m_Radius;
	//YŽ²‚ð‚‚³‚Ì”¼•ª‚É
	XMVectorSetY(vector, m_Height / 2);
	//Å‘åÀ•W‚ÆÅ¬À•W‚ÌŽZo
	XMVECTOR maximumPositionTarget = vector;
	XMVECTOR minimumPositionTarget = -maximumPositionTarget;

	//’†S“_“¯Žm‚ÌƒxƒNƒgƒ‹
	
	//XMVECTOR position = XMLoadFloat3(&m_CenterPosition);
	//position = { max(XMVectorGetX(minimumPositionTarget), min(XMVectorGetX(position), XMVectorGetX(maximumPositionTarget))), max(XMVectorGetY(minimumPositionTarget), min(XMVectorGetY(position), XMVectorGetY(maximumPositionTarget))), max(XMVectorGetZ(minimumPositionTarget), min(XMVectorGetZ(position), XMVectorGetZ(maximumPositionTarget))) };
	//XMFLOAT3 calculatedVector;
	//XMStoreFloat3(&calculatedVector, minimumPositionTarget - position);

	//return calculatedVector.x * calculatedVector.x + calculatedVector.y * calculatedVector.y + calculatedVector.z * calculatedVector.z < target.GetRadius() * target.GetRadius();

	XMVECTOR calculatedVector = XMLoadFloat3(&m_CenterPosition);
	calculatedVector = { max(XMVectorGetX(minimumPositionTarget), min(XMVectorGetX(calculatedVector), XMVectorGetX(maximumPositionTarget))), max(XMVectorGetY(minimumPositionTarget), min(XMVectorGetY(calculatedVector), XMVectorGetY(maximumPositionTarget))), max(XMVectorGetZ(minimumPositionTarget), min(XMVectorGetZ(calculatedVector), XMVectorGetZ(maximumPositionTarget))) };
	calculatedVector -= XMLoadFloat3(&target.GetPosition());

	return XMVectorGetX(XMVector3LengthSq(calculatedVector)) < target.GetRadius() * target.GetRadius();
}

bool Collision::Capsule::isOverlap(const Sphere& target) const
{
	//ã‰º•”•ª‚Ì“–‚½‚è”»’è
	if (target.GetPosition().y > m_CenterPosition.y + m_Height / 2.0f)
	{
		return Sphere{ {m_CenterPosition.x, m_CenterPosition.y, m_CenterPosition.z}, m_Radius }.isOverlap(target);
	}
	else if (target.GetPosition().y < m_CenterPosition.y - m_Height / 2.0f)
	{
		return Sphere{ {m_CenterPosition.x, m_CenterPosition.y, m_CenterPosition.z}, m_Radius }.isOverlap(target);
	}
	//’†‰›‰~’Œ•”•ª‚Ì”»’è
	float cx = m_CenterPosition.x - target.GetPosition().x;
	float cz = m_CenterPosition.z - target.GetPosition().z;
	float r = m_Radius + target.GetRadius();
	return cx * cx + cz * cz <= r * r;
}
