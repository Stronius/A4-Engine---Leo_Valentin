#include <iostream>
#include <A4-TD_LV/GameManager.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/Sprite.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/SpriteSheet.hpp>
#include <A4Engine/SpriteSheetComponent.hpp>
#include <A4-TD_LV/Lavatrap.hpp>
#include <A4Engine/InputManager.hpp>

GameManager::GameManager(entt::registry& registry) :
my_registry(registry)
{
	if (my_instance != nullptr)
		throw std::runtime_error("only one GameManager can be created");

	my_instance = this;
	isPause = false;
	referenceTimerSpawn = 1.5f;
	timerSpawn = 1;
	enemyList = std::vector<Enemy>();
	nbSpawnEnemyToSpawn = 15;
	numEnemy = 0;
	gameLose = false;

	/*CreateTrap({ 576,576 });
	CreateTrap({ 576+128,576+128 });
	CreateTrap({ 576+128,576-128 });
	CreateTrap({ 576+256,576 - 256 });
	CreateTrap({ 576+128,576 - 256 });
	CreateTrap({ 576+256,576 - 256 });
	CreateTrap({ 576+128,576 - 256 });
	CreateTrap({ 576+256,576-256 });*/
	//CreateEnemy({ 576-128,576 });
	//CreateEnemy({ 192.f,128 * 2 + 64 });
	//CreateEnemy({ 192.f,128 * 3 + 64 });
	//CreateEnemy({ 192.f,128 * 4 + 64 });
	//CreateEnemy({ 192.f,128 * 5 + 64 });

	std::shared_ptr<Sprite> tempPauseSprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Pause.png"), -1);
	tempPauseSprite->SetOrigin({ 0.5f, 0.5f });
	tempPauseSprite->SetRect(SDL_Rect{ 0, 0, 512, 512 });
	pauseEntity = GameManager::Instance().my_registry.create();
	GameManager::Instance().my_registry.emplace<GraphicsComponent>(pauseEntity, std::move(tempPauseSprite));
	auto& transformDisplayLavaTrapNumberIndicator = GameManager::Instance().my_registry.emplace<Transform>(pauseEntity);

	transformDisplayLavaTrapNumberIndicator.SetPosition({800, 450});

}

GameManager::~GameManager()
{
	my_instance = nullptr;
}

void GameManager::Pause()
{
	isPause = !isPause;
	if (isPause)
	{
		std::shared_ptr<Sprite> tempPauseSprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Pause.png"), 2);
		my_registry.get<GraphicsComponent>(pauseEntity).renderable = tempPauseSprite;
		tempPauseSprite->SetOrigin({ 0.5f, 0.5f });
		tempPauseSprite->SetRect(SDL_Rect{ 0, 0, 512, 512 });
		my_registry.get<Transform>(pauseEntity).SetPosition({ 800, 450 });
	}
	else
	{
		std::shared_ptr<Sprite> tempPauseSprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Pause.png"), -1);
		my_registry.get<GraphicsComponent>(pauseEntity).renderable = tempPauseSprite;
		tempPauseSprite->SetOrigin({ 0.5f, 0.5f });
		tempPauseSprite->SetRect(SDL_Rect{ 0, 0, 512, 512 });
		my_registry.get<Transform>(pauseEntity).SetPosition({ 800, 450 });
	}
}

void GameManager::Victory()
{
	std::cout << "Victory" << std::endl;
	isPause = true;
	phaseAttack = false;


	//Afficher le sprite de win
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Victoire.png"), 3);
	entt::entity entity = my_registry.create();
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->SetColor(0, 1, 0, 1);
	my_registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = my_registry.emplace<Transform>(entity);
	transform.SetPosition({800, 450});
}

void GameManager::Lose()
{
	isPause = true;


	//Afficher le sprite de lose
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Defaite.png"), 3);
	entt::entity entity = my_registry.create();
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->SetColor(1, 0, 0, 1);
	my_registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = my_registry.emplace<Transform>(entity);
	transform.SetPosition({ 800, 450 });

}

void GameManager::EnemiesMovement(float deltaTime)
{
	bool checkForWin = true;

	auto view = my_registry.view<Enemy,Transform>();
	for (entt::entity entity : view)
	{
		Enemy& enemyScript = view.get<Enemy>(entity);
		Transform& enemyTransform = view.get<Transform>(entity);
		enemyScript.Update(deltaTime);
		enemyTransform = enemyScript.myTransform;

		if (!enemyScript.isDying)
			checkForWin = false;
	}

	if (checkForWin && numEnemy >= 15)
		Victory();
}

void GameManager::CheckForSpawn(float deltaTime)
{
	if (timerSpawn > 0)
	{
		timerSpawn -= deltaTime;
	}
	else
	{
		if (nbSpawnEnemyToSpawn > 0)
		{
			int whereToSpawn = rand() % 5;
			//std::cout << whereToSpawn + 1 << std::endl;

			nbSpawnEnemyToSpawn--;
			CreateEnemy({ 64 ,128.f * (whereToSpawn + 1) + 64 }); //+1 pour pas qu'il pop en 0 car le rand commence à 0
		}
		timerSpawn = referenceTimerSpawn;
	}
}


void GameManager::CreateEnemy(Vector2f pos)
{
	numEnemy++;
	std::shared_ptr<Sprite> enemySprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Enemy.png"),2);
	entt::entity entity = my_registry.create();
	enemySprite->SetOrigin({ 0.5f, 0.5f });
	my_registry.emplace<GraphicsComponent>(entity, std::move(enemySprite));
	my_registry.emplace<Transform>(entity);
	Enemy& enemyScript = my_registry.emplace<Enemy>(entity, pos, entity, numEnemy);

	enemyList.push_back(enemyScript);
}

void GameManager::Update(float deltaTime)
{
	if (phaseAttack) 
	{
		EnemiesMovement(deltaTime);
		CheckForSpawn(deltaTime);
	}
}

GameManager& GameManager::Instance()
{
	if (my_instance == nullptr)
		throw std::runtime_error("GameManager hasn't been instantied");

	return *my_instance;
}

GameManager* GameManager::my_instance = nullptr;
