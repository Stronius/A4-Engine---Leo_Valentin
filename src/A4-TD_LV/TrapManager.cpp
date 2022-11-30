#include <A4-TD_LV/TrapManager.hpp>
#include <A4-TD_LV/GameManager.hpp>
#include <A4-TD_LV/Trap.hpp>
#include <A4Engine/InputManager.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/SpritesheetComponent.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/Color.hpp>
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

	InputManager::Instance().BindKeyPressed(SDLK_RETURN, "Validate");
	InputManager::Instance().BindKeyPressed(SDLK_LSHIFT, "EndSetup1");


	nbrLavaTrapLeft = 12;
	nbrArrowWallLeft = 3;

	for (int i = 0; i < 16; i++)
	{
		std::shared_ptr<Sprite> tempSprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Chiffre" + std::to_string(i) + ".png"), 1);
		tempSprite->SetOrigin({ 0.5f, 0.5f });
		tempSprite->Resize(64, 64);
		tempSprite->SetRect(SDL_Rect{ 0, 0, 128, 128 });
		spriteNumberList.push_back(tempSprite);
		

	}

	std::shared_ptr<Sprite> tempSpriteLava = spriteNumberList[nbrLavaTrapLeft];
	displayLavaTrapNumber = GameManager::Instance().my_registry.create();
	GameManager::Instance().my_registry.emplace<GraphicsComponent>(displayLavaTrapNumber, std::move(tempSpriteLava));
	auto& transformDisplayLavaTrapNumberIndicator = GameManager::Instance().my_registry.emplace<Transform>(displayLavaTrapNumber);
	transformDisplayLavaTrapNumberIndicator.SetPosition({ 640+64,64 });
	
	std::shared_ptr<Sprite> tempSpriteWall = spriteNumberList[nbrArrowWallLeft];
	displayArrowWallTrapNumber = GameManager::Instance().my_registry.create();
	GameManager::Instance().my_registry.emplace<GraphicsComponent>(displayArrowWallTrapNumber, std::move(tempSpriteWall));
	auto& transformDisplayArrowWallTrapNumberIndicator = GameManager::Instance().my_registry.emplace<Transform>(displayArrowWallTrapNumber);
	transformDisplayArrowWallTrapNumberIndicator.SetPosition({ 992,64 });


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
			AttackPhase(deltaTime);
			break;
		}
	}
}

void TrapManager::SetupPhase(float deltaTime)
{
	if (InputManager::Instance().IsActive("EndSetup1") && InputManager::Instance().IsActive("Row4"))
	{
		for (int j = 0; j < selectedIcon.size(); j++)
		{
			selectedIcon[j].renderable->orderLayer = -2;
		}

		currentPhase = Attack;
		GameManager::Instance().phaseAttack = true;
		return;
	}

	InputDetectionPlace();
}

void TrapManager::AttackPhase(float deltaTime)
{
	InputDetectionActivate();
	TrapUpdate(deltaTime);
}

void TrapManager::TrapUpdate(float deltaTime) 
{
	auto view = GameManager::Instance().my_registry.view<Trap, Transform>();
	for (entt::entity entity : view)
	{
		Trap& trap = view.get<Trap>(entity);

		trap.Update(deltaTime);
	}
}

void TrapManager::InputDetectionPlace()
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

	if (pos.x > 0 && pos.y > 0 && InputManager::Instance().IsPressed("Validate"))
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
				nbrLavaTrapLeft++;
				GameManager::Instance().my_registry.get<GraphicsComponent>(displayLavaTrapNumber).renderable = spriteNumberList[nbrLavaTrapLeft];
				foundATrap = true;
				break;
			}
		}

		if (!foundATrap && nbrLavaTrapLeft > 0)
			CreateTrapdoor(GameManager::Instance().my_registry, pos);
	}
	else if (pos.x > 0 && pos.y == 0 && InputManager::Instance().IsPressed("Validate"))
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
				nbrArrowWallLeft++;
				GameManager::Instance().my_registry.get<GraphicsComponent>(displayArrowWallTrapNumber).renderable = spriteNumberList[nbrArrowWallLeft];
				foundATrap = true;
				break;
			}
		}

		if (!foundATrap && nbrArrowWallLeft > 0)
		CreateArrowWall(GameManager::Instance().my_registry, { pos.x+5, 449 });
	}
}

void TrapManager::InputDetectionActivate()
{
	float x = 0, y = 0;

	for (int i = 1; i < 11; i++)
	{
		if (InputManager::Instance().IsPressed("Column" + std::to_string(i)))
		{
			x = 128 * i + 192;
		}
	}

	for (int i = 1; i < 6; i++)
	{
		if (InputManager::Instance().IsPressed("Row" + std::to_string(i)))
		{
			y = 128 * i + 64;
		}
	}

	if (x != 0)
	{
		Vector2f position;

		for each (entt::entity entity in trapdoors)
		{
			position = GameManager::Instance().my_registry.get<Transform>(entity).GetPosition();

			if (abs(position.x - x) < 20)
			{
				GameManager::Instance().my_registry.get<Trap>(entity).Activation();
			}
		}
	}
	else if (y != 0)
	{
		Vector2f position;

		for each (entt::entity entity in trapdoors)
		{
			position = GameManager::Instance().my_registry.get<Transform>(entity).GetPosition();

			if (position.y == y)
			{
				GameManager::Instance().my_registry.get<Trap>(entity).Activation();
			}
		}
	}
}

void TrapManager::CreateTrapdoor(entt::registry& registry, Vector2f pos)
{
	std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>();
	spritesheet->AddAnimation("Idle", 1, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("Open", 5, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("Reset", 5, 100, Vector2i{ 0, 128 }, Vector2i{ 128, 128 });

	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Trapdoor_Anim.png"), 1);
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->Resize(128, 128);
	sprite->SetRect(SDL_Rect{ 0, 0, 128, 128 });
	nbrLavaTrapLeft--;
	registry.get<GraphicsComponent>(displayLavaTrapNumber).renderable = spriteNumberList[nbrLavaTrapLeft];


	entt::entity entity = registry.create();
	registry.emplace<SpritesheetComponent>(entity, spritesheet, sprite);
	registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = registry.emplace<Transform>(entity);
	auto& trap = registry.emplace<Trap>(entity, entity, Vector2f(64.f, 64.f), 4, 4);
	trap.myPosition = pos;
	transform.SetPosition(pos);

	trapdoors.push_back(entity);
}

void TrapManager::CreateArrowWall(entt::registry& registry, Vector2f pos)
{
	std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>();
	spritesheet->AddAnimation("Idle", 2, 100, Vector2i{ 0, 0 }, Vector2i{ 138, 658 });
	spritesheet->AddAnimation("Open", 5, 100, Vector2i{ 138, 0 }, Vector2i{ 138, 658 });
	spritesheet->AddAnimation("Reset", 2, 100, Vector2i{ 0, 0 }, Vector2i{ 138, 658 });

	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/ArrowWallv2.png"), 2);
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->Resize(138, 658);
	sprite->SetRect(SDL_Rect{ 0, 0, 138, 658 });
	nbrArrowWallLeft--;
	registry.get<GraphicsComponent>(displayArrowWallTrapNumber).renderable = spriteNumberList[nbrArrowWallLeft];



	entt::entity entity = registry.create();
	registry.emplace<SpritesheetComponent>(entity, spritesheet, sprite);
	registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = registry.emplace<Transform>(entity);
	auto& trap = registry.emplace<Trap>(entity, entity, Vector2f( 64, 900 ), 1, 5);
	trap.myPosition = pos;
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
