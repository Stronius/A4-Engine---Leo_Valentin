#include <A4Engine/Enemy.hpp>
#include <time.h>
#include <stdlib.h>
#include <iostream>

Enemy::Enemy()
{
	myPosition = Vector2f(0, 0);
	myRotation = 0;
	myTransform = Transform();
	myTransform.SetPosition(myPosition);
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

		std::cout << "enemy1 position = " << tempWaypoint << "\n";

		if (tempWaypoint.x > 1600)
		{
			keepOnSearching = false;
		}
	}
}

void Enemy::NextMovement(int random)
{

}

void Enemy::ApplyMovement(Vector2f nextPosition)
{
	myTransform.Translate(nextPosition);
}
