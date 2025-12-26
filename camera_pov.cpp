#include "camera_pov.h"

void PovCamera::Update()
{
	XMVECTOR shake = { 0.0f, m_ShakeMultiplier * cosf(m_ShakeTime), 0.0f };
	SetPosition(GetPosition() + shake);
	m_ShakeTime += XM_2PI / 3.0f;
	m_ShakeMultiplier *= 0.9f;

	XMMATRIX view = XMMatrixLookAtLH(GetPosition(), m_BallPosition, {0.0f, 1.0f, 0.0f});
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);

	SetViewMatrix(view);
	SetPerspectiveMatrix(perspective);
}

void PovCamera::ShakeStart(float shakeMultiplier)
{
	m_ShakeMultiplier = shakeMultiplier;
}
