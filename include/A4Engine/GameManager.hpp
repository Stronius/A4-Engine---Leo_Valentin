#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Vector2.hpp>
#include <A4Engine/Enemy.hpp>



class A4ENGINE_API GameManager
{
	public:

		bool isPause;

		GameManager();
		GameManager(const GameManager&) = delete;
		GameManager(GameManager&&) = delete;
		~GameManager();

		void Pause();
		void Victory();
		void Lose();
		void EnemiesMovement();

		void Update();

		static GameManager& Instance();


	private:

		std::vector<Enemy> enemyList;
		static GameManager* my_instance;
};
