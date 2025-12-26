#include "camera_move.h"
#include "keylogger.hpp"

void MovingCamera::Update()
{
	//è„à⁄ìÆ
	if (GetKeyDown('Q'))
	{
		SetPosition(GetPosition() + m_VecUp * 0.1f);
	}

	//â∫à⁄ìÆ
	if (GetKeyDown('E'))
	{
		SetPosition(GetPosition() + m_VecDown * 0.1f);
	}

	//ëOà⁄ìÆ
	if (GetKeyDown('W'))
	{

		SetPosition(GetPosition() + m_VecFront * 0.1f);
	}

	//å„ÇÎà⁄ìÆ
	if (GetKeyDown('S'))
	{
		SetPosition(GetPosition() + m_VecFront * -0.1f);
	}

	//âEà⁄ìÆ
	if (GetKeyDown('D'))
	{
		SetPosition(GetPosition() + m_VecRight * 0.1f);
	}

	//ç∂à⁄ìÆ
	if (GetKeyDown('A'))
	{
		SetPosition(GetPosition() + m_VecRight * -0.1f);
	}

	//è„âÒì]
	if (GetKeyDown(VK_UP))
	{
		XMMATRIX rotation = XMMatrixRotationAxis(m_VecRight, XMConvertToRadians(-1));
		m_VecFront = XMVector3TransformNormal(m_VecFront, rotation);
		m_VecRight = XMVector3Cross(m_VecUp, m_VecFront);
	}
	
	//â∫âÒì]
	if (GetKeyDown(VK_DOWN))
	{
		XMMATRIX rotation = XMMatrixRotationAxis(m_VecRight, XMConvertToRadians(1));
		m_VecFront = XMVector3TransformNormal(m_VecFront, rotation);
		m_VecRight = XMVector3Cross(m_VecUp, m_VecFront);
	}

	//âEâÒì]
	if (GetKeyDown(VK_RIGHT))
	{
		XMMATRIX rotation = XMMatrixRotationY(XMConvertToRadians(1));
		m_VecFront = XMVector3TransformNormal(m_VecFront, rotation);
		m_VecRight = XMVector3Cross(m_VecUp, m_VecFront);
	}

	//ç∂âÒì]
	if (GetKeyDown(VK_LEFT))
	{
		XMMATRIX rotation = XMMatrixRotationY(XMConvertToRadians(-1));
		m_VecFront = XMVector3TransformNormal(m_VecFront, rotation);
		m_VecRight = XMVector3Cross(m_VecUp, m_VecFront);
	}

	m_VecFront = XMVector3Normalize(m_VecFront);
	m_VecRight = XMVector3Normalize(m_VecRight);
	m_VecUp = XMVector3Normalize(m_VecUp);
	m_VecDown = XMVector3Normalize(m_VecDown);

	XMMATRIX view = XMMatrixLookAtLH(GetPosition(), GetPosition() + m_VecFront, m_VecUp);
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);

	SetViewMatrix(view);
	SetPerspectiveMatrix(perspective);
}
