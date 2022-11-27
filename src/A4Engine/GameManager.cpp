#include <A4Engine/GameManager.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/Sprite.hpp>
#include <A4Engine/GraphicsComponent.hpp>



GameManager::GameManager(entt::registry& registry) :
my_registry(registry)
{
	if (my_instance != nullptr)
		throw std::runtime_error("only one GameManager can be created");

	my_instance = this;
	isPause = false;
	referenceTimerSpawn = 2.f;
	timerSpawn = 0;
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

void GameManager::EnemiesMovement(float deltaTime)
{
	auto view = my_registry.view<Enemy,Transform>();
	for (entt::entity entity : view)
	{
		Enemy& enemyScript = view.get<Enemy>(entity);
		Transform& enemyTransform = view.get<Transform>(entity);
		enemyScript.Update(deltaTime);
		enemyTransform = enemyScript.myTransform;
	}
}

void GameManager::CheckForSpawn(float deltaTime)
{
	if (timerSpawn > 0)
	{
		timerSpawn -= deltaTime;
	}
	else
	{
		CreateEnemy();
		timerSpawn = referenceTimerSpawn;
	}
}


void GameManager::CreateEnemy()
{
	std::shared_ptr<Sprite> enemySprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Enemy.png"));
	entt::entity entity = my_registry.create();
	my_registry.emplace<GraphicsComponent>(entity, std::move(enemySprite));
	my_registry.emplace<Transform>(entity);
	my_registry.emplace<Enemy>(entity);
}

void GameManager::Update(float deltaTime)
{
	EnemiesMovement(deltaTime);
	CheckForSpawn(deltaTime);
}

GameManager& GameManager::Instance()
{
	if (my_instance == nullptr)
		throw std::runtime_error("GameManager hasn't been instantied");

	return *my_instance;
}

GameManager* GameManager::my_instance = nullptr;
