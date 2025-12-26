#pragma once

#include "camera.h"

class MovingCamera : public Camera
{
private:
	XMVECTOR m_VecUp = { 0.0f, 1.0f, 0.0f };
	XMVECTOR m_VecDown = { 0.0f, -1.0f, 0.0f };
	XMVECTOR m_VecFront = { 0.0f, 0.0f, 1.0f };
	XMVECTOR m_VecRight = { 1.0f, 0.0f, 0.0f };

public:
	MovingCamera() = default;
	MovingCamera(const XMVECTOR& position) { SetPosition(position); }

	void Update() override;
};