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

		GameManager(entt::registry& registry);
		GameManager(const GameManager&) = delete;
		GameManager(GameManager&&) = delete;
		~GameManager();

		void Pause();
		void Victory();
		void Lose();
		void EnemiesMovement(float deltaTime);
		void CheckForSpawn(float deltaTime);
		void CreateEnemy();

		void Update(float deltaTime);

		static GameManager& Instance();


	private:

		std::vector<Enemy> enemyList;
		static GameManager* my_instance;
};
