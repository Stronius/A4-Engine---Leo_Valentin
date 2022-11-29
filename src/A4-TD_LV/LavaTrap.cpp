#include <iostream>
#include <A4-TD_LV/LavaTrap.hpp>
#include <A4-TD_LV/GameManager.hpp>
#include <A4Engine/SpritesheetComponent.hpp>


LavaTrap::LavaTrap(entt::entity gameObject) :
myPosition(Vector2f(0, 0)),
cooldown(3),
enemyDetected(false),
isActive(false),
gameObject(gameObject)
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

			//spritesheetComponent.PlayAnimation("Open");

			GameManager::Instance().my_registry.get<SpritesheetComponent>(gameObject).PlayAnimation("Open");
			std::cout << "enemy Detected" << std::endl;
		}
		else if (abs(myPosition.x - enemyTransform.GetPosition().x) > 64 && abs(myPosition.y - enemyTransform.GetPosition().y) > 64 && isActive)	//Faut pas le faire ici sinon il va le faire instant mais c'est juste pour test des trucs
		{
			isActive = false;
			
			//spritesheetComponent.PlayAnimation("Close");

			GameManager::Instance().my_registry.get<SpritesheetComponent>(gameObject).PlayAnimation("Close");
			std::cout << "enemy out" << std::endl;
		}

	}

}

void LavaTrap::Reloading()
{

}
