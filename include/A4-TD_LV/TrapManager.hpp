#pragma once
#include <entt/entt.hpp>
#include <A4Engine/Vector2.hpp>


class TrapManager
{
	public:
		TrapManager();
		~TrapManager();

		void Update(float deltaTime);
		void SetupPhase(float deltaTime);
		void AttackPhase(float deltaTime);
		void CreateTrapdoor(entt::registry& registry, Vector2f pos);

		enum phases {Setup, Attack};
		phases currentPhase;
		Vector2f pos;
	private:

};