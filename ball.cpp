#include "ball.h"
#include "light.h"

void Ball::LoadModel()
{
	m_pModel = ::ModelLoad("asset/model/ball.fbx"); //ModelLoadの重複を::で回避
}

void Ball::Update()
{
	m_Position += m_Acceleration;

	m_Acceleration += {0.0f, -0.001f, 0.0f}; 

	if (m_isOverlap)
	{
		m_Acceleration = { 0.0f, 0.0f, 0.0f };
	}

	m_isOverlap = false;
}

void Ball::Draw() const
{
	XMMATRIX world = XMMatrixTranslationFromVector(m_Position);
	SetLight(world);
	ModelDraw(m_pModel, world);
}

//void Ball::OnHit(const XMFLOAT3& face, const Collision::AABB& sourceAABB, const Collision::AABB& targetAABB)
const void Ball::OnHit(const Collision::AABB& targetAABB,int type, float e)
{
    Collision::AABB aabb = targetAABB;

    switch (type)
    {
    case 0:
        aabb.SetMaxY(aabb.GetCenter().y + GetAABB().GetCenter().z - aabb.GetCenter().z);
        break;
    case 1:
        aabb.SetMaxY(aabb.GetCenter().y + -(GetAABB().GetCenter().x - aabb.GetCenter().x));
        break;
    case 2:
        aabb.SetMaxY(aabb.GetCenter().y + -(GetAABB().GetCenter().z - aabb.GetCenter().z));
        break;
    case 3:
        aabb.SetMaxY(aabb.GetCenter().y + GetAABB().GetCenter().x - aabb.GetCenter().x);
        break;
    }

    if (GetAABB().isOverlap(aabb))
    {
        //cubeのAABBの中心座標からボールの中心座標へのベクトルの一番長い軸を算出
        XMVECTOR cubeToBallVector = XMLoadFloat3(&GetAABB().GetCenter()) - XMLoadFloat3(&aabb.GetCenter());

        float x = XMVectorGetX(cubeToBallVector);
        float y = XMVectorGetY(cubeToBallVector);
        float z = XMVectorGetZ(cubeToBallVector);

        float ax = fabsf(x);
        float ay = fabsf(y);
        float az = fabsf(z);

        bool isMaxLengthX = false;
        bool isMaxLengthY = false;
        bool isMaxLengthZ = false;

        XMFLOAT3 hitFace{ 0.0f, 0.0f, 0.0f };

        if (ax > ay)
        {
            if (ax > az)
            {
                isMaxLengthX = true;
            }
            else
            {
                isMaxLengthZ = true;
            }
        }
        else
        {
            if (ay > az)
            {
                isMaxLengthY = true;
            }
            else
            {
                isMaxLengthZ = true;
            }
        }

        float m = 0.0f;
        if (isMaxLengthX)
        {
            if (x > 0)
            {
                m = aabb.GetMaxSurface().x - (GetAABB().GetMinSurface().x - GetAABB().GetCenter().x);
            }
            else
            {
                m = aabb.GetMinSurface().x - (GetAABB().GetMaxSurface().x - GetAABB().GetCenter().x);
            }

            SetPosition(XMVectorSetX(GetPosition(), m));
            m_Acceleration = XMVectorSetX(m_Acceleration, -XMVectorGetX(m_Acceleration));
        }
        else if (isMaxLengthY)
        {
            if (y > 0)
            {
                m = aabb.GetMaxSurface().y - (GetAABB().GetMinSurface().y - GetAABB().GetCenter().y);

                const float time = 1.0f / 60.0f;
                switch (type)
                {
                case 0:
                    m_Acceleration = XMVectorSetZ(m_Acceleration, XMVectorGetZ(m_Acceleration) + -0.5f * time * -XMVectorGetY(m_Acceleration) * e * 500.0f);
                    m_Acceleration = XMVectorSetY(m_Acceleration, 0.0f);
                    break;
                case 1:
                    m_Acceleration = XMVectorSetX(m_Acceleration, XMVectorGetX(m_Acceleration) + 0.5 * time * XMVectorGetY(m_Acceleration) * e * 500.0f);
                    m_Acceleration = XMVectorSetY(m_Acceleration, 0.0f);
                    break;
                case 2:
                    m_Acceleration = XMVectorSetZ(m_Acceleration, XMVectorGetZ(m_Acceleration) + 0.5 * time * -XMVectorGetY(m_Acceleration) * e * 500.0f);
                    m_Acceleration = XMVectorSetY(m_Acceleration, 0.0f);
                    break;
                case 3:
                    m_Acceleration = XMVectorSetX(m_Acceleration, XMVectorGetX(m_Acceleration) + -0.5f * time * XMVectorGetY(m_Acceleration) * e * 500.0f);
                    m_Acceleration = XMVectorSetY(m_Acceleration, 0.0f);
                    break;
                default:
                    m_Acceleration = XMVectorSetY(m_Acceleration, -XMVectorGetY(m_Acceleration));
                    break;
                }
            }
            else
            {
                m = aabb.GetMinSurface().y - (GetAABB().GetMaxSurface().y - GetAABB().GetCenter().y);
                m_Acceleration = XMVectorSetY(m_Acceleration, -XMVectorGetY(m_Acceleration));
            }

            SetPosition(XMVectorSetY(GetPosition(), m));
        }
        else if (isMaxLengthZ)
        {
            if (z > 0)
            {
                m = aabb.GetMaxSurface().z - (GetAABB().GetMinSurface().z - GetAABB().GetCenter().z);
            }
            else
            {
                m = aabb.GetMinSurface().z - (GetAABB().GetMaxSurface().z - GetAABB().GetCenter().z);
            }

            SetPosition(XMVectorSetZ(GetPosition(), m));
            m_Acceleration = XMVectorSetZ(m_Acceleration, -XMVectorGetZ(m_Acceleration));
        }

        m_Acceleration *= e;
    }
}
