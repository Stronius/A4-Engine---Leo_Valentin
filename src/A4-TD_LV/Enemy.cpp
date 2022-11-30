#include <A4-TD_LV/Enemy.hpp>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <A4-TD_LV/GameManager.hpp>

Enemy::Enemy(Vector2f pos, entt::entity entity, int num)
{
	myTransform = Transform();
	myTransform.SetPosition(pos);
	myPosition = Vector2f(0, 0);
	myRotation = 0;
	currentWaypoint = 0;
	moveSpeed = 1.f;
	isDying = false;
	myEntity = entity;
	myNum = num;

	WaypointsCalculation();
	direction = (waypoints[currentWaypoint] - myTransform.GetPosition()) * 0.014;

}

Enemy::~Enemy()
{

}

void Enemy::GetKill()
{
	if (myTransform.GetScale().x <= 0.01)
	{
		myTransform.SetPosition(Vector2f(-500, 0));
	}
}

void Enemy::ScaleDown(float deltaTime)
{
	if (myTransform.GetScale().x > 0.01)
	{
		Vector2f tempScale = myTransform.GetScale() - Vector2f(deltaTime, deltaTime) * 1;
		myTransform.SetScale(tempScale);

		GetKill();
	}
}

void Enemy::WaypointsCalculation()
{
	srand(time(NULL));

	int tempX = myTransform.GetPosition().x;
	int tempY = myTransform.GetPosition().y;

	bool keepOnSearching = true;
	while (keepOnSearching)
	{
		int forwardOrNot = rand() % 100;

		if (forwardOrNot >= 50)
		{
			tempX += 128;
		}
		else if (forwardOrNot < 50)
		{
			int upOrDown = rand() % 100;

			bool goUp = false;

			if (upOrDown >= 50)
			{
				goUp = true;
			}
			else
			{
				goUp = false;
			}

			if (tempY == 192) //clamp haut
			{
				goUp = false;
			}
			else if (tempY == (128 * 5) + 64) //clamp bas
			{
				goUp = true;
			}

			if (goUp)
			{
				tempY -= 128;
			}
			else
			{
				tempY += 128;
			}
		}

		Vector2f tempWaypoint(tempX, tempY);

		waypoints.push_back(tempWaypoint);



		if (tempWaypoint.x >= 1600)
		{
			keepOnSearching = false;
		}
	}
}

void Enemy::GoToNextWaypoint(float deltaTime)
{
	
	if (abs(myTransform.GetPosition().x - waypoints[currentWaypoint].x) <= 5 && abs(myTransform.GetPosition().y - waypoints[currentWaypoint].y) <= 5)
	{
		if (deltaTime <= 0)
		{
			return;
		}
		if (currentWaypoint >= waypoints.size())
		{
			direction = Vector2f(0, 0);
			currentWaypoint = 0;
			return;
		}
		currentWaypoint++;
		direction = (waypoints[currentWaypoint] - myTransform.GetPosition()) * deltaTime * moveSpeed;
	}
	else
	{
		if (currentWaypoint >= waypoints.size() && !GameManager::Instance().gameLose)
		{
			GameManager::Instance().gameLose = true;
			std::cout << "Lose" << std::endl;
			GameManager::Instance().Lose();
		}
		myTransform.Translate(direction);
	}
}

void Enemy::Update(float deltaTime)
{
	if (isDying)
	{
		ScaleDown(deltaTime);
	}
	else
	{
		GoToNextWaypoint(deltaTime);
	}
}
