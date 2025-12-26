#include "camera_easing.h"

void EasingCamera::Update()
{
	m_Position = m_CurrentPosition;

	if (m_IsStart)
	{
		float t = (float)m_frameCount / m_MovingFrameTime;

		float e = easeInOutQuint(t);

		m_Position = m_CurrentPosition + (m_TargetPosition - m_CurrentPosition) * e;

		m_frameCount = (std::min)(++m_frameCount, m_MovingFrameTime);
	}

	XMMATRIX view = XMMatrixLookAtLH(m_Position, m_Target->GetPosition(), { 0.0f, 1.0f, 0.0f });
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);

	SetViewMatrix(view);
	SetPerspectiveMatrix(perspective);
}
