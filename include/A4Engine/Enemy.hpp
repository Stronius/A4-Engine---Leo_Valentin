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
		int currentWaypoint;
		float moveSpeed;
		Vector2f direction;


		Enemy();
		~Enemy();

		void WaypointsCalculation();
		void GoToNextWaypoint(float deltaTime);
		void Update(float deltaTime);

	private:
		std::vector<Vector2f> waypoints;
};
