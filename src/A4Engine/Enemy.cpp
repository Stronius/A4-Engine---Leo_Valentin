#include <A4Engine/Enemy.hpp>
#include <time.h>
#include <stdlib.h>
#include <iostream>

Enemy::Enemy()
{
	myTransform = Transform();
	myTransform.SetPosition({ 192, 192 });
	myPosition = Vector2f(0, 0);
	myRotation = 0;
	currentWaypoint = 0;
	moveSpeed = 2.f;
	WaypointsCalculation();
	direction = Vector2f(0, 0);
	direction = (waypoints[currentWaypoint] - myTransform.GetPosition()) * 0.01f;

}

Enemy::~Enemy()
{

}

void Enemy::WaypointsCalculation()
{
	srand(time(NULL));

	int tempX = 192;
	int tempY = 192;

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

			if (tempY == 128 + 64)
			{
				goUp = false;
				std::cout << "Ne peut pas aller plus haut \n";

			}
			else if (tempY == (128 * 5) + 64)
			{
				goUp = true;
				std::cout << "Ne peut pas aller plus BAS \n";
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



		if (tempWaypoint.x > 1600)
		{
			keepOnSearching = false;
		}
	}
}

void Enemy::GoToNextWaypoint(float deltaTime)
{
	
	if (abs(myTransform.GetPosition().x - waypoints[currentWaypoint].x) <= 10 && abs(myTransform.GetPosition().y - waypoints[currentWaypoint].y) <= 10)
	{
		if (currentWaypoint >= waypoints.size())
		{
			direction = Vector2f(0, 0);
			currentWaypoint = 0;
			return;
		}
		currentWaypoint++;
		std::cout << "Prochain waypoints" << std::endl;
		direction = (waypoints[currentWaypoint] - myTransform.GetPosition()) * deltaTime * moveSpeed;
	}
	else
	{
		std::cout << "enemy1 position = " << myTransform.GetPosition() << std::endl;
		myTransform.Translate(direction);
	}
}

void Enemy::Update(float deltaTime)
{
	GoToNextWaypoint(deltaTime);
}
