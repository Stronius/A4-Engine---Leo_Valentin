#include <A4Engine/GameManager.hpp>


GameManager::GameManager()
{
	if (my_instance != nullptr)
		throw std::runtime_error("only one GameManager can be created");

	my_instance = this;
}

GameManager::~GameManager()
{
	my_instance = nullptr;
}

GameManager& GameManager::Instance()
{
	if (my_instance == nullptr)
		throw std::runtime_error("GameManager hasn't been instantied");

	return *my_instance;
}

GameManager* GameManager::my_instance = nullptr;
