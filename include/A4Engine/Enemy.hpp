#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Vector2.hpp>
#include <A4Engine/Transform.hpp>


class A4ENGINE_API Enemy
{
	public:
		Transform myTransform;
		Vector2f myPosition;
		float myRotation;

		Enemy();
		~Enemy();

		void WaypointsCalculation();
		void NextMovement(int random);
		void ApplyMovement(Vector2f nextPosition);

	private:
		std::vector<Vector2f> waypoints;
};
