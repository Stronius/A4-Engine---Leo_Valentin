#include <iostream>
#include <SDL.h>
#include <A4Engine/AnimationSystem.hpp>
#include <A4Engine/CameraComponent.hpp>
#include <A4Engine/ChipmunkBody.hpp>
#include <A4Engine/ChipmunkShape.hpp>
#include <A4Engine/ChipmunkSpace.hpp>
#include <A4Engine/CollisionShape.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/InputManager.hpp>
#include <A4Engine/Model.hpp>
#include <A4Engine/PhysicsSystem.hpp>
#include <A4Engine/RenderSystem.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/RigidBodyComponent.hpp>
#include <A4Engine/SDLpp.hpp>
#include <A4Engine/SDLppImGui.hpp>
#include <A4Engine/SDLppRenderer.hpp>
#include <A4Engine/SDLppTexture.hpp>
#include <A4Engine/SDLppWindow.hpp>
#include <A4Engine/Sprite.hpp>
#include <A4Engine/SpritesheetComponent.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/VelocityComponent.hpp>
#include <A4Engine/VelocitySystem.hpp>
#include <A4-TD_LV/Enemy.hpp>
#include <A4-TD_LV/GameManager.hpp>
#include <A4-TD_LV/LavaTrap.hpp>
#include <A4-TD_LV/TrapManager.hpp>
#include <chipmunk/chipmunk.h>
#include <entt/entt.hpp>
#include <fmt/core.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

entt::entity CreateCamera(entt::registry& registry);
entt::entity CreateBackground(entt::registry& registry);
entt::entity CreateTrap(entt::registry& registry, Vector2f pos);


void EntityInspector(const char* windowName, entt::registry& registry, entt::entity entity);

void HandleCameraMovement(entt::registry& registry, entt::entity camera, float deltaTime);

int main()
{
	SDLpp sdl;

	SDLppWindow window("A4Engine", 1600, 900);
	SDLppRenderer renderer(window, "direct3d11", SDL_RENDERER_PRESENTVSYNC);

	ResourceManager resourceManager(renderer);
	InputManager inputManager;

	SDLppImGui imgui(window, renderer);

	ImGui::SetCurrentContext(imgui.GetContext());

	// Touches directionnelles (cam�ra)
	InputManager::Instance().BindKeyPressed(SDLK_LEFT, "CameraMoveLeft");
	InputManager::Instance().BindKeyPressed(SDLK_RIGHT, "CameraMoveRight");


	//Pose de pi�ges
	InputManager::Instance().BindKeyPressed(SDLK_1, "Column1");
	InputManager::Instance().BindKeyPressed(SDLK_2, "Column2");
	InputManager::Instance().BindKeyPressed(SDLK_3, "Column3");
	InputManager::Instance().BindKeyPressed(SDLK_4, "Column4");
	InputManager::Instance().BindKeyPressed(SDLK_5, "Column5");
	InputManager::Instance().BindKeyPressed(SDLK_6, "Column6");
	InputManager::Instance().BindKeyPressed(SDLK_7, "Column7");
	InputManager::Instance().BindKeyPressed(SDLK_8, "Column8");
	InputManager::Instance().BindKeyPressed(SDLK_9, "Column9");
	InputManager::Instance().BindKeyPressed(SDLK_0, "Column10");
	InputManager::Instance().BindKeyPressed(SDLK_a, "Row1");
	InputManager::Instance().BindKeyPressed(SDLK_z, "Row2");
	InputManager::Instance().BindKeyPressed(SDLK_e, "Row3");
	InputManager::Instance().BindKeyPressed(SDLK_r, "Row4");
	InputManager::Instance().BindKeyPressed(SDLK_t, "Row5");


	//Valider
	InputManager::Instance().BindKeyPressed(SDLK_KP_ENTER, "EndSetup");



	std::shared_ptr<Spritesheet> spriteSheet = std::make_shared<Spritesheet>();
	spriteSheet->AddAnimation("idle", 5, 0.1f, Vector2i{ 0, 0 },  Vector2i{ 32, 32 });
	spriteSheet->AddAnimation("run",  8, 0.1f, Vector2i{ 0, 32 }, Vector2i{ 32, 32 });
	spriteSheet->AddAnimation("jump", 4, 0.1f, Vector2i{ 0, 64 }, Vector2i{ 32, 32 });

	entt::registry registry;

	AnimationSystem animSystem(registry);
	RenderSystem renderSystem(renderer, registry);
	VelocitySystem velocitySystem(registry);
	PhysicsSystem physicsSystem(registry);
	physicsSystem.SetGravity({ 0.f, 981.f });
	physicsSystem.SetDamping(0.9f);
	GameManager gameManager(registry);
	TrapManager trapManager;

	entt::entity cameraEntity = CreateCamera(registry);

	/*entt::entity enemy1 = CreateEnemy(registry);
	registry.get<Enemy>(enemy1).myTransform.SetPosition({ 128.f, 128.f });*/

	//entt::entity trapdoor = CreateTrap(registry, { 512, 512 });

	entt::entity background = CreateBackground(registry);

	Uint64 lastUpdate = SDL_GetPerformanceCounter();

	bool isOpen = true;
	while (isOpen)
	{
		Uint64 now = SDL_GetPerformanceCounter();
		float deltaTime = (float) (now - lastUpdate) / SDL_GetPerformanceFrequency();
		lastUpdate = now;
		
		SDL_Event event;
		while (SDLpp::PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				isOpen = false;

			imgui.ProcessEvent(event);

			InputManager::Instance().HandleEvent(event);
		}

		imgui.NewFrame();

		renderer.SetDrawColor(127, 0, 127, 255);
		renderer.Clear();

		HandleCameraMovement(registry, cameraEntity, deltaTime);

		trapManager.Update(deltaTime);
		gameManager.Update(deltaTime);


		animSystem.Update(deltaTime);
		velocitySystem.Update(deltaTime);
		physicsSystem.Update(deltaTime);
		renderSystem.Update(deltaTime);
		InputManager::Instance().Update();



		ImGui::LabelText("FPS", "%f", 1.f / deltaTime);

		EntityInspector("Camera", registry, cameraEntity);

		physicsSystem.DebugDraw(renderer, registry.get<Transform>(cameraEntity).GetTransformMatrix().Inverse());

		imgui.Render();

		renderer.Present();
	}

	return 0;
}

void EntityInspector(const char* windowName, entt::registry& registry, entt::entity entity)
{
	Transform& transform = registry.get<Transform>(entity);

	float rotation = transform.GetRotation();
	Vector2f pos = transform.GetPosition();
	Vector2f scale = transform.GetScale();

	ImGui::Begin(windowName);

	ImGui::LabelText("Position", "X: %f\nY: %f", pos.x, pos.y);

	if (ImGui::SliderFloat("Rotation", &rotation, -180.f, 180.f))
		transform.SetRotation(rotation);

	float scaleVal[2] = { scale.x, scale.y };
	if (ImGui::SliderFloat2("Scale", scaleVal, -5.f, 5.f))
		transform.SetScale({ scaleVal[0], scaleVal[1] });

	if (ImGui::Button("Reset"))
	{
		transform.SetScale({ 1.f, 1.f });
		transform.SetRotation(0.f);
		transform.SetPosition(Vector2f(0.f, 0.f));
	}

	ImGui::End();
}

entt::entity CreateCamera(entt::registry& registry)
{
	entt::entity entity = registry.create();
	registry.emplace<CameraComponent>(entity);
	registry.emplace<Transform>(entity);

	return entity;
}

entt::entity CreateBackground(entt::registry& registry)
{
	std::shared_ptr<Sprite> background = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/LevelLayout.png"));

	entt::entity entity = registry.create();
	registry.emplace<GraphicsComponent>(entity, std::move(background));
	auto& transform = registry.emplace<Transform>(entity);

	return entity;
}

entt::entity CreateTrap(entt::registry& registry, Vector2f pos)
{
	std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>();
	spritesheet->AddAnimation("idle", 1, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("open", 5, 100, Vector2i{ 0, 0 }, Vector2i{ 128, 128 });
	spritesheet->AddAnimation("close", 5, 100, Vector2i{ 0, 128 }, Vector2i{ 128, 128 });

	std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Trapdoor_Open.png"),1);
	sprite->SetOrigin({ 0.f, 0.f });
	sprite->Resize(128, 128);
	sprite->SetRect(SDL_Rect{ 0, 0, 128, 128 });

	entt::entity entity = registry.create();
	registry.emplace<SpritesheetComponent>(entity, spritesheet, sprite);
	registry.emplace<GraphicsComponent>(entity, std::move(sprite));
	auto& transform = registry.emplace<Transform>(entity);
	registry.emplace<LavaTrap>(entity, entity);
	transform.SetPosition(pos);

	return entity;
}

void HandleCameraMovement(entt::registry& registry, entt::entity camera, float deltaTime)
{
	Transform& cameraTransform = registry.get<Transform>(camera);

	if (InputManager::Instance().IsActive("CameraMoveLeft"))
		cameraTransform.Translate(Vector2f(-500.f * deltaTime, 0.f));

	if (InputManager::Instance().IsActive("CameraMoveRight"))
		cameraTransform.Translate(Vector2f(500.f * deltaTime, 0.f));
}