#pragma once

#include <A4Engine/Vector2.hpp>
#include <A4Engine/Transform.hpp>
#include <entt/entt.hpp>

class Enemy
{
	public:
		Transform myTransform;
		Vector2f myPosition;
		float myRotation;
		int currentWaypoint;
		float moveSpeed;
		Vector2f direction;
		bool isDying;
		entt::entity myEntity;

		Enemy(Vector2f pos, entt::entity entity);
		~Enemy();

		void GetKill();
		void ScaleDown(float deltaTime);
		void WaypointsCalculation();
		void GoToNextWaypoint(float deltaTime);
		void Update(float deltaTime);

	private:
		std::vector<Vector2f> waypoints;
};
