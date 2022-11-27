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
#include <A4Engine/Enemy.hpp>
#include <A4Engine/GameManager.hpp>
#include <chipmunk/chipmunk.h>
#include <entt/entt.hpp>
#include <fmt/core.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

entt::entity CreateCamera(entt::registry& registry);
entt::entity CreateHouse(entt::registry& registry);
entt::entity CreateEnemy(entt::registry& registry);


void EntityInspector(const char* windowName, entt::registry& registry, entt::entity entity);

void HandleCameraMovement(entt::registry& registry, entt::entity camera, float deltaTime);

int main()
{
	SDLpp sdl;

	SDLppWindow window("A4Engine", 1600, 900);
	SDLppRenderer renderer(window, "direct3d11", SDL_RENDERER_PRESENTVSYNC);

	ResourceManager resourceManager(renderer);
	InputManager inputManager;
	GameManager gameManager;

	SDLppImGui imgui(window, renderer);

	ImGui::SetCurrentContext(imgui.GetContext());

	// ZQSD
	InputManager::Instance().BindKeyPressed(SDLK_q, "MoveLeft");
	InputManager::Instance().BindKeyPressed(SDLK_d, "MoveRight");
	InputManager::Instance().BindKeyPressed(SDLK_z, "MoveUp");
	InputManager::Instance().BindKeyPressed(SDLK_s, "MoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_8, "Move8");

	// Touches directionnelles (caméra)
	InputManager::Instance().BindKeyPressed(SDLK_LEFT, "CameraMoveLeft");
	InputManager::Instance().BindKeyPressed(SDLK_RIGHT, "CameraMoveRight");
	InputManager::Instance().BindKeyPressed(SDLK_UP, "CameraMoveUp");
	InputManager::Instance().BindKeyPressed(SDLK_DOWN, "CameraMoveDown");

	//Pose de pièges
	InputManager::Instance().BindKeyPressed(SDLK_0, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_1, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_2, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_3, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_4, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_5, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_6, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_7, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_8, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_9, "CameraMoveDown");

	InputManager::Instance().BindKeyPressed(SDLK_a, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_z, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_e, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_r, "CameraMoveDown");
	InputManager::Instance().BindKeyPressed(SDLK_t, "CameraMoveDown");

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

	entt::entity cameraEntity = CreateCamera(registry);

	entt::entity enemy1 = CreateEnemy(registry);
	registry.get<Enemy>(enemy1).myTransform.SetPosition({ 192.f, 192.f });

	entt::entity house = CreateHouse(registry);
	registry.get<RigidBodyComponent>(house).TeleportTo({ 750.f, 275.f });
	registry.get<Transform>(house).SetScale({ 2.f, 2.f });

	// Création du sol
	std::shared_ptr<CollisionShape> groundShape = std::make_shared<SegmentShape>(Vector2f(0.f, 720.f), Vector2f(10'000.f, 720.f));
	
	InputManager::Instance().BindKeyPressed(SDLK_SPACE, "Jump");

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
		//HandleRunnerMovement(registry, runner, deltaTime);


		if (!gameManager.isPause)
		{
			registry.get<Enemy>(enemy1).Update(deltaTime);
			registry.get<Transform>(enemy1).SetPosition(registry.get<Enemy>(enemy1).myTransform.GetPosition());
		}

		animSystem.Update(deltaTime);
		velocitySystem.Update(deltaTime);
		physicsSystem.Update(deltaTime);
		renderSystem.Update(deltaTime);



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

entt::entity CreateHouse(entt::registry& registry)
{
	std::shared_ptr<Model> house = ResourceManager::Instance().GetModel("assets/house.model");

	std::shared_ptr<CollisionShape> collider = std::make_shared<ConvexShape>(*house, Matrix3f::Scale({ 2.f, 2.f }));

	entt::entity entity = registry.create();
	registry.emplace<GraphicsComponent>(entity, std::move(house));
	registry.emplace<Transform>(entity);

	auto& entityPhysics = registry.emplace<RigidBodyComponent>(entity, RigidBodyComponent::Static{});
	entityPhysics.AddShape(std::move(collider));

	return entity;
}

entt::entity CreateEnemy(entt::registry& registry)
{
	std::shared_ptr<Sprite> enemySprite = std::make_shared<Sprite>(ResourceManager::Instance().GetTexture("assets/Enemy.png"));
	entt::entity entity = registry.create();
	registry.emplace<GraphicsComponent>(entity, std::move(enemySprite));
	auto& transform = registry.emplace<Transform>(entity);
	auto& enemy = registry.emplace<Enemy>(entity);
	enemy.myTransform = transform;

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