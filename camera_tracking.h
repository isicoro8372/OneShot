#pragma once

#include "camera.h"

class Ball;

class TrackingCamera : public Camera
{
private:
	Ball* m_Target = nullptr;
	float m_Length = 1.0f;
	float m_PositionHight = 1.0f;
	float m_Angle = 0.0f;

public:
	TrackingCamera() = default;

	void SetTarget(Ball* target) { m_Target = target; };

	void Update() override;
};