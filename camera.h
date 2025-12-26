#pragma once

#include "main.h"

#define DEFAULT_CAMERA_MAX (10)

class Camera
{
protected:
	XMMATRIX m_MtxView;
	XMMATRIX m_MtxPerspective;
	XMVECTOR m_Position;

public:
	Camera() : m_Position({ 5.0f, 5.0f, -5.0f }) {};

	virtual void Update();

	void SetViewMatrix(const XMMATRIX& mtx);
	void SetPerspectiveMatrix(const XMMATRIX& mtx);
	void SetPosition(const XMVECTOR& position) { m_Position = position; }

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetPerspectiveMatrix() const;
	const XMMATRIX& GetCameraMatrix() const;
	const XMVECTOR& GetPosition() { return m_Position; }
};