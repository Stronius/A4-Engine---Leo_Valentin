#include <A4-TD_LV/TrapManager.hpp>
#include <A4-TD_LV/GameManager.hpp>
#include <A4Engine/InputManager.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/SpritesheetComponent.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/Sprite.hpp>
#include <iostream>

TrapManager::TrapManager()
{
	currentPhase = Setup;
	pos = { 0, 0 };

	CreateSelectedIcon(GameManager::Instance().my_registry, { 320 + 8, 832 + 10});
	CreateSelectedIcon(GameManager::Instance().my_registry, { 448 + 8, 832 + 10 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 576 + 8, 832 + 10 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 704 + 8, 832 + 10 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 832 + 8, 832 + 10 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 960 + 8, 832 + 10 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 1088 + 8, 832 + 10 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 1216 + 8, 832 + 10 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 1344 + 8, 832 + 10 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 1472 + 8, 832 + 10 });

	CreateSelectedIcon(GameManager::Instance().my_registry, { 1728 + 8, 192-6 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 1728 + 8, 320-2 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 1728 + 8, 448+2 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 1728 + 8, 576+6 });
	CreateSelectedIcon(GameManager::Instance().my_registry, { 1728 + 8, 704+10 });

	InputManager::Instance().BindKeyPressed(SDLK_1, "Column1");		// x : 256 	   +64		320
	InputManager::Instance().BindKeyPressed(SDLK_2, "Column2");		// x : 384	   +64		448
	InputManager::Instance().BindKeyPressed(SDLK_3, "Column3");	    // x : 512	   +64		576
	InputManager::Instance().BindKeyPressed(SDLK_4, "Column4");	    // x : 640	   +64		704
	InputManager::Instance().BindKeyPressed(SDLK_5, "Column5");	    // x : 768	   +64		832
	InputManager::Instance().BindKeyPressed(SDLK_6, "Column6");	    // x : 896	   +64		960
	InputManager::Instance().BindKeyPressed(SDLK_7, "Column7");	    // x : 1024	   +64		1088
	InputManager::Instance().BindKeyPressed(SDLK_8, "Column8");	    // x : 1152	   +64		1216
	InputManager::Instance().BindKeyPressed(SDLK_9, "Column9");	    // x : 1280	   +64		1344
	InputManager::Instance().BindKeyPressed(SDLK_0, "Column10");	// x : 1408    +64		1472

	InputManager::Instance().BindKeyPressed(SDLK_a, "Row1");		// y : 128	   +64		192
	InputManager::Instance().BindKeyPressed(SDLK_z, "Row2");		// y : 256	   +64		320
	InputManager::Instance().BindKeyPressed(SDLK_e, "Row3");		// y : 384	   +64		448
	InputManager::Instance().BindKeyPressed(SDLK_r, "Row4");		// y : 512	   +64		576
	InputManager::Instance().BindKeyPressed(SDLK_t, "Row5");		// y : 640	   +64		704

	InputManager::Instance().BindKeyPressed(SDLK_RETURN, "EndSetup");


	nbrLavaTrapLeft = 12;
	nbrArrowWallLeft = 4;

	for (int i = 0; i < 16; i++)
	{
		spriteNumberList.push_back(std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Chiffre" + std::to_string(i) + ".png"), 1));
	}

	displayLavaTrapNumber = spriteNumberList[nbrLavaTrapLeft];
	displayLavaTrapNumber = spriteNumberList[nbrLavaTrapLeft-5];
	displayLavaTrapNumber->SetOrigin({ 0.5f, 0.5f });
	displayLavaTrapNumber->Resize(64, 64);
	displayLavaTrapNumber->SetRect(SDL_Rect{ 0, 0, 128, 128 });
	entt::entity entityDisplayLavaTrapNumberIndicator = GameManager::Instance().my_registry.create();
	GameManager::Instance().my_registry.emplace<GraphicsComponent>(entityDisplayLavaTrapNumberIndicator, std::move(displayLavaTrapNumber));
	auto& transformDisplayLavaTrapNumberIndicator = GameManager::Instance().my_registry.emplace<Transform>(entityDisplayLavaTrapNumberIndicator);
	transformDisplayLavaTrapNumberIndicator.SetPosition({ 640+64,64 });
	
	displayArrowWallTrapNumber = spriteNumberList[nbrArrowWallLeft];
	displayArrowWallTrapNumber->SetOrigin({ 0.5f, 0.5f });
	displayArrowWallTrapNumber->Resize(64, 64);
	displayArrowWallTrapNumber->SetRect(SDL_Rect{ 0, 0, 128, 128 });
	entt::entity entityDisplayArrowWallTrapNumberIndicator = GameManager::Instance().my_registry.create();
	GameManager::Instance().my_registry.emplace<GraphicsComponent>(entityDisplayArrowWallTrapNumberIndicator, std::move(displayArrowWallTrapNumber));
	auto& transformDisplayArrowWallTrapNumberIndicator = GameManager::Instance().my_registry.emplace<Transform>(entityDisplayArrowWallTrapNumberIndicator);
	transformDisplayArrowWallTrapNumberIndicator.SetPosition({ 896+64,96 });


}

TrapManager::~TrapManager()
{
}

void TrapManager::Update(float deltaTime)
{
	switch (currentPhase) 
	{
		case Setup: 
		{
			SetupPhase(deltaTime);
			break;
		}
		case Attack: 
		{
			break;
		}
	}
}

void TrapManager::SetupPhase(float deltaTime)
{
	/*if (InputManager::Instance().IsPressed("EndSetup")) 
	{
		for (int j = 0; j < selectedIcon.size(); j++)
		{
			selectedIcon[j].renderable->orderLayer = -2;
		}

		currentPhase = Attack;
	}*/

	InputDetection();

}

void TrapManager::InputDetection()
{
	for (int i = 1; i < 11; i++)
	{
		if (InputManager::Instance().IsPressed("Column" + std::to_string(i)))
		{

			for (int j = 0; j < 10; j++)
			{
				if (j == i - 1) 
				{
					if (selectedIcon[j].renderable->orderLayer == 1) 
					{
						pos.x = 0;
						selectedIcon[j].renderable->orderLayer = -2;
					}
					else 
					{
						pos.x = 128 * i + 192;
						selectedIcon[j].renderable->orderLayer = 1;
					}
				}
				else
					selectedIcon[j].renderable->orderLayer = -2;
			}
		}
	}

	for (int i = 1; i < 6; i++)
	{
		if (InputManager::Instance().IsPressed("Row" + std::to_string(i)))
		{
			for (int j = 10; j < 15; j++)
			{
				if (j == i + 9)
				{
					if (selectedIcon[j].renderable->orderLayer == 1)
					{
						pos.y = 0;
						selectedIcon[j].renderable->orderLayer = -2;
					}
					else
					{
						pos.y = 128 * i + 64;
						selectedIcon[j].renderable->orderLayer = 1;
					}
				}
				else
					selectedIcon[j].renderable->orderLayer = -2;
			}
		}
	}

	if (pos.x > 0 && pos.y > 0 && InputManager::Instance().IsPressed("EndSetup"))
	{
		Vector2f position;
		bool foundATrap = false;

		for each (entt::entity entity in trapdoors)
		{
			position = GameManager::Instance().my_registry.get<Transform>(entity).GetPosition();

			if (position.x == pos.x && position.y == pos.y)
			{
				auto e = std::find(trapdoors.begin(), trapdoors.end(), entity);
				trapdoors.erase(e);

				GameManager::Instance().my_registry.destroy(entity);
				foundATrap = true;
				break;
			}
		}

		if (!foundATrap && nbrLavaTrapLeft > 0)
			CreateTrapdoor(GameManager::Instance().my_registry, pos);
	}
	else if (pos.x > 0 && pos.y == 0 && InputManager::Instance().IsPressed("EndSetup"))
	{
		Vector2f position;
		bool foundATrap = false;

		for each (entt::entity entity in trapdoors)
		{
			position = GameManager::Instance().my_registry.get<Transform>(entity).GetPosition();

			if (position.x == pos.x + 5 && position.y == 449)
			{
				auto e = std::find(trapdoors.begin(), trapdoors.end(), entity);
				trapdoors.erase(e);

				GameManager::Instance().my_registry.destroy(entity);
				foundATrap = true;
				break;
			}
		}

		if (!foundATrap && nbrArrowWallLeft > 0)
		CreateArrowWall(GameManager::Instance().my_registry, { pos.x+5, 449 });
	}
}

void TrapManager::CreateTrapdoor(entt::registry& registry, Vector2f pos)
{
	std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>();
	spritesheet->AddAnimation("Idle", 1, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("Open", 5, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("Close", 5, 100, Vector2i{ 0, 128 }, Vector2i{ 128, 128 });

	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Trapdoor_Anim.png"), 1);
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->Resize(128, 128);
	sprite->SetRect(SDL_Rect{ 0, 0, 128, 128 });
	nbrLavaTrapLeft--;
	displayLavaTrapNumber = spriteNumberList[nbrLavaTrapLeft];

	entt::entity entity = registry.create();
	registry.emplace<SpritesheetComponent>(entity, spritesheet, sprite);
	registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = registry.emplace<Transform>(entity);
	transform.SetPosition(pos);

	trapdoors.push_back(entity);
}

void TrapManager::CreateArrowWall(entt::registry& registry, Vector2f pos)
{
	std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>();
	spritesheet->AddAnimation("Idle", 1, 100, Vector2i{ 0, 0 }, Vector2i{ 138, 658 });
	spritesheet->AddAnimation("Shoot", 5, 100, Vector2i{ 0, 0 }, Vector2i{ 138, 658 });

	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/ArrowWall.png"), 2);
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->Resize(138, 658);
	sprite->SetRect(SDL_Rect{ 0, 0, 138, 658 });
	nbrArrowWallLeft--;
	displayLavaTrapNumber = spriteNumberList[nbrArrowWallLeft];


	entt::entity entity = registry.create();
	registry.emplace<SpritesheetComponent>(entity, spritesheet, sprite);
	registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = registry.emplace<Transform>(entity);
	transform.SetPosition(pos);

	trapdoors.push_back(entity);
}

void TrapManager::CreateSelectedIcon(entt::registry& registry, Vector2f pos) 
{
	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/SelectedIcon.png"), -2);
	sprite->SetOrigin({ 0.5f, 0.5f });
	entt::entity entity = registry.create();
	auto graphic = registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = registry.emplace<Transform>(entity);
	selectedIcon.push_back(graphic);
	transform.SetPosition(pos);
}
