#pragma once

#include "Ball.h"
#include "camera.h"

class EasingCamera : public Camera
{
private:
	Ball* m_Target = nullptr;
	XMVECTOR m_CurrentPosition = { 0.0f, 0.0f, 0.0f };
	XMVECTOR m_TargetPosition = { 0.0f, 0.0f, 0.0f };
	int m_MovingFrameTime = 0;
	int m_frameCount = 0;
	bool m_IsStart = false;

	float easeOutBack(float t)
	{
		return 1 + (--t) * t * (2.70158 * t + 1.70158);
	}
	float easeInOutQuint(float t)
	{
		float t2;
		if (t < 0.5)
		{
			t2 = t * t;
			return 16 * t * t2 * t2;
		}
		else
		{
			t2 = (--t) * t;
			return 1 + 16 * t * t2 * t2;
		}
	}

public:
	EasingCamera() = default;
	EasingCamera(const XMVECTOR& currentPosition, const XMVECTOR& targetPosition, int movingFrameTime) : m_CurrentPosition(currentPosition), m_TargetPosition(targetPosition), m_MovingFrameTime(movingFrameTime) {};

	void SetTarget(Ball* target) { m_Target = target; };
	void SetMovingFrameTime(int time) { m_MovingFrameTime = time; }
	void MoveStart() { m_IsStart = true; m_frameCount = 0, m_TargetPosition = m_Target->GetPosition() + XMVECTOR{ 0.0f, 3.0f, 0.0f }; }
	void MoveStart(XMVECTOR& targetPosition) { m_IsStart = true; m_frameCount = 0; m_TargetPosition = targetPosition; }
	void MoveStart(const XMVECTOR& currentPosition, const XMVECTOR& targetPosition) { m_IsStart = true; m_frameCount = 0; m_CurrentPosition = currentPosition, m_TargetPosition = targetPosition; }

	void Update() override;
};