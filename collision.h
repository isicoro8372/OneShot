#pragma once
#include "main.h"
#include "renderer.h"

constexpr float COLLISION_RADIUS = 3.0f;

namespace Collision
{
	class AABB;
	class Point;
	class Sphere;
	class Cylinder;
	class Capsule;

	class Interface
	{
	public:
		virtual bool isOverlap(const AABB& target) const = 0;
		virtual bool isOverlap(const Point& target) const = 0;
		virtual bool isOverlap(const Sphere& target) const = 0;
		virtual bool isOverlap(const Cylinder& target) const = 0;
		virtual bool isOverlap(const Capsule& target) const = 0;
	};


	class AABB : public Interface
	{
		XMFLOAT3 m_MinSurfacePos = { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 m_MaxSurfacePos = { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 m_Center = { 0.0f, 0.0f, 0.0f };

	public:
		AABB() = default;
		AABB(const XMFLOAT3& min, const XMFLOAT3& max);

		void SetMaxY(float y) { m_MaxSurfacePos.y = y; }

		const XMFLOAT3& GetMinSurface() const { return m_MinSurfacePos; }
		void SetMinSurface(const XMFLOAT3& min) { m_MinSurfacePos = min;	}

		const XMFLOAT3& GetMaxSurface() const { return m_MaxSurfacePos; }
		void SetMaxSurface(const XMFLOAT3& max) { m_MaxSurfacePos = max; }

		const XMFLOAT3& GetCenter() const { return m_Center; }

		bool isOverlap(const AABB& target) const override;
		bool isOverlap(const Point& target) const override;
		bool isOverlap(const Sphere& target) const override { return false; }
		bool isOverlap(const Cylinder& target) const override { return false; }
		bool isOverlap(const Capsule& target) const override { return false; }

		AABB Translation(const XMVECTOR& translate) const;
	};

	class Point : public Interface
	{
		XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	public:
		Point() = default;
		Point(const XMFLOAT3& position) : m_Position(position) {};
	
		void SetPosition(const XMFLOAT3& position) { m_Position = position; }

		const XMFLOAT3& GetPosition() const	{ return m_Position; }

		bool isOverlap(const AABB& target) const override { return target.isOverlap(*this); }
		bool isOverlap(const Point& target) const override { return false;	}
		bool isOverlap(const Sphere& target) const override { return false; }
		bool isOverlap(const Cylinder& target) const override { return false; }
		bool isOverlap(const Capsule& target) const override { return false; }

	};

	class Sphere : public Interface
	{
		XMFLOAT3 m_CenterPosition = { 0.0f,0.0f,0.0f };
		FLOAT m_Radius = 5.0f;
	public:
		Sphere() = default;
		Sphere(const XMFLOAT3& center, FLOAT radius) : m_CenterPosition(center), m_Radius(radius) {}
		
		const XMFLOAT3& GetPosition() const { return m_CenterPosition; }
		const FLOAT GetRadius() const { return m_Radius; }

		Sphere Translation(const XMVECTOR& translate) const;
		
		bool isOverlap(const AABB& target) const override { return false;	}
		bool isOverlap(const Point& target) const override;
		bool isOverlap(const Sphere& target) const override;
		bool isOverlap(const Cylinder& target) const override;
		bool isOverlap(const Capsule& target) const override;
	};

	class Cylinder : public Interface
	{
		FLOAT m_Radius = 0.25f;
		FLOAT m_Height = 3.0f;
		XMFLOAT3 m_CenterPosition = { 0.0f,0.0f,0.0f };
	public:
		Cylinder() = default;
		Cylinder(const XMFLOAT3& center, FLOAT height, FLOAT radius) : m_CenterPosition(center), m_Height(height), m_Radius(radius) {}

		const XMFLOAT3& GetPosition() const { return m_CenterPosition; }
		const FLOAT GetRadius() const { return m_Radius; }

		bool isOverlap(const AABB& target) const override { return false; }
		bool isOverlap(const Point& target) const override { return false; }
		bool isOverlap(const Sphere& target) const override;
		bool isOverlap(const Cylinder& target) const override { return false; }
		bool isOverlap(const Capsule& target) const override { return false; }

	};

	class Capsule : public Interface
	{
		FLOAT m_Radius = 0.25f;
		FLOAT m_Height = 3.0f;
		XMFLOAT3 m_CenterPosition = { 0.0f,0.0f,0.0f };
	public:
		Capsule() = default;
		Capsule(const XMFLOAT3& center, FLOAT height, FLOAT radius) : m_CenterPosition(center), m_Height(height), m_Radius(radius) {}

		const XMFLOAT3& GetPosition() const { return m_CenterPosition; }
		const FLOAT GetRadius() const { return m_Radius; }

		bool isOverlap(const AABB& target) const override { return false; }
		bool isOverlap(const Point& target) const override { return false; }
		bool isOverlap(const Sphere& target) const override;
		bool isOverlap(const Cylinder& target) const override { return false; }
		bool isOverlap(const Capsule& target) const override { return false; }
	};
};
