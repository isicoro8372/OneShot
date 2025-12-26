#include "camera.h"

void Camera::SetViewMatrix(const XMMATRIX& mtx)
{
	m_MtxView = mtx;
}

void Camera::SetPerspectiveMatrix(const XMMATRIX& mtx)
{
	m_MtxPerspective = mtx;
}

void Camera::Update()
{
	SetPosition({ 5.0f, 5.0f, -5.0f });
	m_MtxView = XMMatrixLookAtLH(m_Position, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	m_MtxPerspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return m_MtxView;
}

const XMMATRIX& Camera::GetPerspectiveMatrix() const
{
	return m_MtxPerspective;
}

const XMMATRIX& Camera::GetCameraMatrix() const
{
	return m_MtxView * m_MtxPerspective;
}
