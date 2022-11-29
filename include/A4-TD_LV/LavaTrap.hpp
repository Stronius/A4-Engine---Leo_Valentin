#pragma once

#include <entt/entt.hpp>
#include <A4Engine/Vector2.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/SpritesheetComponent.hpp>


class LavaTrap
{
	public:
		entt::entity gameObject;
		Vector2f myPosition;
		bool enemyDetected;
		bool isActive;
		bool trapCoolDown;
		float coolDownRemaining;
		float baseCooldown;

		LavaTrap(entt::entity gameObject);
		~LavaTrap();

		void Update(float deltaTime);
		void Activation(float deltaTime);
		void Reloading(float deltaTime);
};