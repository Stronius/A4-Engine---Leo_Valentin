#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Vector2.hpp>
#include <A4Engine/Enemy.hpp>
#include <entt/entt.hpp>



class A4ENGINE_API GameManager
{
	public:

		bool isPause;
		entt::registry& my_registry;
		float referenceTimerSpawn;
		float timerSpawn;
		bool spawnEnemy;
		std::vector<Enemy> enemyList;

		GameManager(entt::registry& registry);
		GameManager(const GameManager&) = delete;
		GameManager(GameManager&&) = delete;
		~GameManager();

		void Pause();
		void Victory();
		void Lose();
		void EnemiesMovement(float deltaTime);
		void TrapDetection();


		void CheckEnemyTraped(Vector2f trapPosition);

		void CheckForSpawn(float deltaTime);
		void CreateEnemy(Vector2f pos);
		void CreateTrap(Vector2f pos);

		void Update(float deltaTime);

		static GameManager& Instance();


	private:

		static GameManager* my_instance;
};
