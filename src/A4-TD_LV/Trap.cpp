#include <iostream>
#include <A4-TD_LV/Trap.hpp>
#include <A4-TD_LV/GameManager.hpp>
#include <A4Engine/SpritesheetComponent.hpp>
#include <A4Engine/Sprite.hpp>

Trap::Trap(entt::entity gameObject, Vector2f killRange, float activeDuration, float cooldownDuration) :
gameObject(gameObject),
myPosition(Vector2f(0,0)),
m_killRange(killRange),
enemyDetected(false),
isActive(false),
isInCooldown(false),
m_activeLeft(activeDuration),
m_activeDuration(activeDuration),
m_cooldownLeft(cooldownDuration),
m_cooldownDuration(cooldownDuration)
{
}

void Trap::Update(float deltaTime)
{
	if (isActive) 
	{
		KillEnemies(deltaTime);
	}
	else 
	{
		if (isInCooldown)
		{
			Reloading(deltaTime);
		}
	}
}

void Trap::Activation()
{
	if (!isActive && !isInCooldown) 
	{
		GameManager::Instance().my_registry.get<SpritesheetComponent>(gameObject).PlayAnimation("Open");
		isActive = true;
	}
}

void Trap::Reloading(float deltaTime)
{
	if (m_cooldownLeft > 0)
	{
		m_cooldownLeft -= deltaTime;
	}
	else
	{
		isInCooldown = false;
		m_cooldownLeft = m_cooldownDuration;
		GameManager::Instance().my_registry.get<SpritesheetComponent>(gameObject).PlayAnimation("Idle");
		GameManager::Instance().my_registry.get<SpritesheetComponent>(gameObject).SetColor(1, 1, 1, 1);
	}
}

void Trap::KillEnemies(float deltaTime)
{
	if (m_activeLeft > 0) 
	{
		m_activeLeft -= deltaTime;
	}
	else 
	{
		isActive = false;
		isInCooldown = true;
		m_activeLeft = m_activeDuration;
		GameManager::Instance().my_registry.get<SpritesheetComponent>(gameObject).PlayAnimation("Reset");
		GameManager::Instance().my_registry.get<SpritesheetComponent>(gameObject).SetColor(0.3, 0.3, 0.3, 1);
	}

	//tuer tout les ennemis detecté a porté ici tant que isActive est true
	auto view = GameManager::Instance().my_registry.view<Enemy, Transform>();
	for (entt::entity entity : view)
	{
		Transform& enemyTransform = view.get<Transform>(entity);
		Enemy& enemy = view.get<Enemy>(entity);

		if (abs(myPosition.x - enemyTransform.GetPosition().x) <= m_killRange.x && abs(myPosition.y - enemyTransform.GetPosition().y) <= m_killRange.y)
		{

			if (!enemy.isDying)
				enemy.isDying = true;
		}
	}
}
