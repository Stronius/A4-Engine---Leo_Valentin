#include <iostream>
#include <A4Engine/LavaTrap.hpp>
#include <A4Engine/GameManager.hpp>


LavaTrap::LavaTrap() :
myPosition(Vector2f(0,0)),
cooldown(3),
enemyDetected(false),
isActive(false)
{

}


LavaTrap::~LavaTrap()
{

}

void LavaTrap::Update()
{
	Activation();
}

void LavaTrap::Activation()
{
	auto view = GameManager::Instance().my_registry.view<Enemy, Transform>();
	for (entt::entity entity : view)
	{
		Enemy& enemy = view.get<Enemy>(entity);
		Transform& enemyTransform = view.get<Transform>(entity);

		if (abs(myPosition.x - enemyTransform.GetPosition().x) <= 64 && abs(myPosition.y - enemyTransform.GetPosition().y) <= 64 && !isActive)
		{
			isActive = true;

			std::cout << "enemy Detected" << std::endl;
		}
	}

}

void LavaTrap::Reloading()
{

}
