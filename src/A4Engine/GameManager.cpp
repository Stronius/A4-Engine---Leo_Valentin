#include <A4Engine/GameManager.hpp>


GameManager::GameManager()
{
	if (my_instance != nullptr)
		throw std::runtime_error("only one GameManager can be created");

	my_instance = this;
	isPause = false;
}

GameManager::~GameManager()
{
	my_instance = nullptr;
}

void GameManager::Pause()
{
	isPause = !isPause;
}

void GameManager::Victory()
{

}

void GameManager::Lose()
{

}

void GameManager::EnemiesMovement()
{
	for (auto it = enemyList.begin(); it != enemyList.end(); )
	{

	}
}

void GameManager::Update()
{
	EnemiesMovement();
}

GameManager& GameManager::Instance()
{
	if (my_instance == nullptr)
		throw std::runtime_error("GameManager hasn't been instantied");

	return *my_instance;
}

GameManager* GameManager::my_instance = nullptr;
