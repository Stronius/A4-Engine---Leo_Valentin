#pragma once

#include <entt/entt.hpp>
#include <A4Engine/Vector2.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/SpritesheetComponent.hpp>


class Trap
{
	public:
		entt::entity gameObject;
		Vector2f myPosition;
		Vector2f m_killRange;
		bool enemyDetected;
		bool isActive;
		bool isInCooldown;
		float m_activeLeft;
		float m_activeDuration; 
		float m_cooldownLeft;
		float m_cooldownDuration;
	
		Trap(entt::entity gameObject, Vector2f killRange, float activeDuration, float cooldownDuration);
		~Trap() = default;
	
		void Update(float deltaTime);
		void Activation();
		void Reloading(float deltaTime);
		void KillEnemies(float deltaTime);
};