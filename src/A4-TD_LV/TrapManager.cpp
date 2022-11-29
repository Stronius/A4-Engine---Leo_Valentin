#include <A4-TD_LV/TrapManager.hpp>
#include <A4Engine/InputManager.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/SpritesheetComponent.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/Sprite.hpp>

TrapManager::TrapManager()
{
	currentPhase = Setup;
	pos = { 0, 0 };

	InputManager::Instance().BindKeyPressed(SDLK_1, "Column1");		// x : 256
	InputManager::Instance().BindKeyPressed(SDLK_2, "Column2");		// x : 384
	InputManager::Instance().BindKeyPressed(SDLK_3, "Column3");	    // x : 512
	InputManager::Instance().BindKeyPressed(SDLK_4, "Column4");	    // x : 640
	InputManager::Instance().BindKeyPressed(SDLK_5, "Column5");	    // x : 768
	InputManager::Instance().BindKeyPressed(SDLK_6, "Column6");	    // x : 896
	InputManager::Instance().BindKeyPressed(SDLK_7, "Column7");	    // x : 1024
	InputManager::Instance().BindKeyPressed(SDLK_8, "Column8");	    // x : 1152
	InputManager::Instance().BindKeyPressed(SDLK_9, "Column9");	    // x : 1280
	InputManager::Instance().BindKeyPressed(SDLK_0, "Column10");	// x : 1404
	InputManager::Instance().BindKeyPressed(SDLK_a, "Row1");		// y :
	InputManager::Instance().BindKeyPressed(SDLK_z, "Row2");		// y :
	InputManager::Instance().BindKeyPressed(SDLK_e, "Row3");		// y :
	InputManager::Instance().BindKeyPressed(SDLK_r, "Row4");		// y :
	InputManager::Instance().BindKeyPressed(SDLK_t, "Row5");		// y :

	InputManager::Instance().BindKeyPressed(SDLK_KP_ENTER, "EndSetup");

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
	if (InputManager::Instance().IsPressed("EndSetup"))
		currentPhase = Attack;



}

void TrapManager::CreateTrapdoor(entt::registry& registry, Vector2f pos)
{
	std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>();
	spritesheet->AddAnimation("idle", 1, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("open", 5, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("close", 5, 100, Vector2i{ 0, 128 }, Vector2i{ 128, 128 });

	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Trapdoor_Anim.png"), 0);
	sprite->SetOrigin({ 0.5f, 0.5f });
	sprite->Resize(128, 128);
	sprite->SetRect(SDL_Rect{ 0, 0, 128, 128 });

	entt::entity entity = registry.create();
	registry.emplace<SpritesheetComponent>(entity, spritesheet, sprite);
	registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = registry.emplace<Transform>(entity);
	transform.SetPosition(pos);
}
