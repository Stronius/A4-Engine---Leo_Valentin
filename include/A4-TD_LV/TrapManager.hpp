#pragma once
#include <entt/entt.hpp>
#include <A4Engine/Vector2.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/Sprite.hpp>


class TrapManager
{
	public:

		TrapManager();
		~TrapManager();

		void Update(float deltaTime);
		void SetupPhase(float deltaTime);
		void AttackPhase(float deltaTime);
		void TrapUpdate(float deltaTime);

		void InputDetectionPlace();
		void InputDetectionActivate();

		void CreateTrapdoor(entt::registry& registry, Vector2f pos);
		void CreateArrowWall(entt::registry& registry, Vector2f pos);
		void CreateSelectedIcon(entt::registry& registry, Vector2f pos);

		void SaveConfiguration();
		void LoadConfiguration();

		int nbrLavaTrapLeft;
		int nbrArrowWallLeft;
		entt::entity displayLavaTrapNumber;
		entt::entity displayArrowWallTrapNumber;
		std::vector<std::shared_ptr<Sprite>> spriteNumberList;

		enum phases {Setup, Attack};
		phases currentPhase;
		Vector2f pos;
		std::vector<GraphicsComponent> selectedIcon;
		std::vector<entt::entity> trapdoors;

	private:

};