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
		float cooldown;

		LavaTrap(entt::entity gameObject);
		~LavaTrap();

		void Update();
		void Activation();
		void Reloading();
};