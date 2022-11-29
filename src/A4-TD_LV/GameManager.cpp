#include <iostream>
#include <A4-TD_LV/GameManager.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/Sprite.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/SpriteSheet.hpp>
#include <A4Engine/SpriteSheetComponent.hpp>
#include <A4-TD_LV/Lavatrap.hpp>

GameManager::GameManager(entt::registry& registry) :
my_registry(registry)
{
	if (my_instance != nullptr)
		throw std::runtime_error("only one GameManager can be created");

	my_instance = this;
	isPause = false;
	referenceTimerSpawn = 3.f;
	timerSpawn = 3;
	enemyList = std::vector<Enemy>();
	deleteMeBool = true;

	CreateTrap({ 576,576 });
	CreateTrap({ 576+128,576+128 });
	CreateTrap({ 576+128,576-128 });
	CreateTrap({ 576+256,576 - 256 });
	CreateTrap({ 576+128,576 - 256 });
	CreateTrap({ 576+256,576 - 256 });
	CreateTrap({ 576+128,576 - 256 });
	CreateTrap({ 576+256,576-256 });
	CreateEnemy({ 576-128,576 });
	CreateEnemy({ 192.f,128 * 2 + 64 });
	CreateEnemy({ 192.f,128 * 3 + 64 });
	CreateEnemy({ 192.f,128 * 4 + 64 });
	CreateEnemy({ 192.f,128 * 5 + 64 });
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

void GameManager::TrapDetection(float deltaTime)
{
	auto view = my_registry.view<LavaTrap>();
	for (entt::entity entity : view)
	{
		LavaTrap& LavaScript = view.get<LavaTrap>(entity);
		//Transform& LavaTransform = view.get<Transform>(entity);
		LavaScript.Update(deltaTime);
		//LavaTransform = LavaScript.myTransform;
	}
}

void GameManager::CheckEnemyTraped(Vector2f trapPosition)
{
	auto it = enemyList.begin();
	if (it != enemyList.end())
	{
		if (abs(trapPosition.x - it->myTransform.GetPosition().x) <= 128 && abs(trapPosition.y - it->myTransform.GetPosition().y) <= 128)
		{
			std::cout << "enemy truc" << std::endl;
		}
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
		if (deleteMeBool)
		{
			deleteMeBool = false;
			//CreateEnemy({ 192.f,128 * 1 + 64 });
		}
		timerSpawn = referenceTimerSpawn;
	}
}


void GameManager::CreateEnemy(Vector2f pos)
{
	std::shared_ptr<Sprite> enemySprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Enemy.png"),2);
	entt::entity entity = my_registry.create();
	enemySprite->SetOrigin({ 0.5f, 0.5f });
	my_registry.emplace<GraphicsComponent>(entity, std::move(enemySprite));
	my_registry.emplace<Transform>(entity);
	Enemy& enemyScript = my_registry.emplace<Enemy>(entity, pos, entity);

	enemyList.push_back(enemyScript);
}

void GameManager::CreateTrap(Vector2f pos)
{
	std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>();
	spritesheet->AddAnimation("Idle", 1, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("Open", 5, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("Close", 5, 100, Vector2i{ 0, 128 }, Vector2i{ 128, 128 });

	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Trapdoor_Anim.png"),1);
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->Resize(128, 128);
	sprite->SetRect(SDL_Rect{ 0, 0, 128, 128 });

	entt::entity entity = my_registry.create();
	auto& spritesheetComponent = my_registry.emplace<SpritesheetComponent>(entity, spritesheet, sprite);
	my_registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = my_registry.emplace<Transform>(entity);
	auto& trapScript = my_registry.emplace<LavaTrap>(entity, entity);
	transform.SetPosition(pos);
	trapScript.myPosition = transform.GetPosition();

}

void GameManager::Update(float deltaTime)
{
	EnemiesMovement(deltaTime);
	CheckForSpawn(deltaTime);
	TrapDetection(deltaTime);
}

GameManager& GameManager::Instance()
{
	if (my_instance == nullptr)
		throw std::runtime_error("GameManager hasn't been instantied");

	return *my_instance;
}

GameManager* GameManager::my_instance = nullptr;
