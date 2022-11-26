#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Vector2.hpp>

class ChipmunkSpace;
struct cpBody;

class A4ENGINE_API ChipmunkBody
{
	public:
		ChipmunkBody(const ChipmunkBody&) = delete;
		ChipmunkBody(ChipmunkBody&& body) noexcept;
		~ChipmunkBody();

		void ApplyImpulse(const Vector2f& impulse);
		void ApplyImpulseAtWorldPoint(const Vector2f& impulse, const Vector2f& worldPoint);

		float GetAngularVelocity() const;
		Vector2f GetCenterOfGravity() const;
		Vector2f GetDirection() const;
		cpBody* GetHandle() const;
		Vector2f GetLinearVelocity() const;
		float GetMass() const;
		float GetMoment() const;
		Vector2f GetPosition() const;
		float GetRotation() const;

		bool IsDynamic() const;
		bool IsKinematic() const;
		bool IsStatic() const;

		void ReindexShapes();

		void SetAngularVelocity(float angularVel);
		void SetCenterOfGravity(const Vector2f& centerOfMass);
		void SetLinearVelocity(const Vector2f& velocity);
		void SetMass(float mass);
		void SetMoment(float moment);
		void SetPosition(const Vector2f& pos);
		void SetRotation(float rotation);

		ChipmunkBody& operator=(const ChipmunkBody&) = delete;
		ChipmunkBody& operator=(ChipmunkBody&& body) noexcept;

		static ChipmunkBody Build(ChipmunkSpace& space, float mass, float moment);
		static ChipmunkBody BuildKinematic(ChipmunkSpace& space);
		static ChipmunkBody BuildStatic(ChipmunkSpace& space);

	private:
		ChipmunkBody(ChipmunkSpace& space, cpBody* body);

		cpBody* m_handle;
};