#pragma once

#include "model.h"
#include "collision.h"

class Ball
{
	Collision::AABB m_Aabb;
	XMVECTOR m_Position;
	XMVECTOR m_Acceleration = { 0.0f,0.0f,0.0f };
	MODEL* m_pModel = nullptr;
	bool m_isOverlap = false;

public:
	Ball() = default;
	Ball(const XMVECTOR& position) : m_Position(position) {};
	~Ball() { ModelRelease(); }

	void Update();
	void Draw() const;

	void AddForce(const XMVECTOR& force)
	{
		m_Acceleration += force;
	}

	void StopMove()
	{
		m_Acceleration = { 0.0f,0.0f,0.0f };
	}

	XMVECTOR GetPosition() const
	{
		return m_Position;
	}

	void SetPosition(const XMVECTOR& position)
	{
		m_Position = position;
	}

	Collision::AABB GetAABB() const
	{
		return m_pModel->aabb.Translation(m_Position);
	}

	void OnOverlap() { m_isOverlap = true; }

	const void OnHit(const Collision::AABB& targetAABB, int type = -1, float e = 0.35f);

	bool IsStopped(bool isHit)
	{
		if (XMVectorGetX(XMVector3Length(m_Acceleration)) < 0.01f && isHit)
		{
			m_Acceleration = { 0.0f, 0.0f, 0.0f };
			return true;
		}
		return false;
	}

	void LoadModel();

	void ModelRelease()
	{
		if (m_pModel)
		{
			::ModelRelease(m_pModel);
			m_pModel = nullptr;
		}
	}
};