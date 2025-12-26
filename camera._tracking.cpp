#include "ball.h"
#include "camera_tracking.h"
#include "keylogger.hpp"

void TrackingCamera::Update()
{
	XMMATRIX view = XMMatrixLookAtLH(m_Position, m_Target->GetPosition(), {0.0f, 1.0f, 0.0f});
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);

	SetViewMatrix(view);
	SetPerspectiveMatrix(perspective);
}
