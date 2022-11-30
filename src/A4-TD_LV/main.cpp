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

	// Touches directionnelles (caméra)
	InputManager::Instance().BindKeyPressed(SDLK_q, "CameraMoveLeft");
	InputManager::Instance().BindKeyPressed(SDLK_d, "CameraMoveRight");
	InputManager::Instance().BindKeyPressed(SDLK_LEFT, "CameraMoveLeftArrow");
	InputManager::Instance().BindKeyPressed(SDLK_RIGHT, "CameraMoveRightArrow");

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

void HandleCameraMovement(entt::registry& registry, entt::entity camera, float deltaTime)
{
	Transform& cameraTransform = registry.get<Transform>(camera);

	if (InputManager::Instance().IsActive("CameraMoveLeft") || InputManager::Instance().IsActive("CameraMoveLeftArrow"))
		cameraTransform.Translate(Vector2f(-500.f * deltaTime, 0.f));

	if (InputManager::Instance().IsActive("CameraMoveRight") || InputManager::Instance().IsActive("CameraMoveRightArrow"))
		cameraTransform.Translate(Vector2f(500.f * deltaTime, 0.f));

	if (cameraTransform.GetPosition().x < 0)
		cameraTransform.SetPosition({ 0, cameraTransform.GetPosition().y });
	else if (cameraTransform.GetPosition().x > 187)
		cameraTransform.SetPosition({ 187, cameraTransform.GetPosition().y });
}