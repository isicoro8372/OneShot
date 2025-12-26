#pragma once

#include "camera.h"

class PovCamera : public Camera
{
	XMVECTOR m_BallPosition;

	float m_ShakeTime = 0.0f;
	float m_ShakeMultiplier = 0.0f;

public:
	PovCamera() = default;
	PovCamera(const XMVECTOR& position) { SetPosition(position); }

	void SetBallPosition(const XMVECTOR& position) { m_BallPosition = position; }
	void Update() override;

	void ShakeStart(float shakeMultiplier);
};