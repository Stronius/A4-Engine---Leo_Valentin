#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Vector2.hpp>
#include <A4Engine/Transform.hpp>


class A4ENGINE_API LavaTrap
{
	public:

		Vector2f myPosition;
		bool enemyDetected;
		bool isActive;
		float cooldown;

		LavaTrap();
		~LavaTrap();

		void Update();
		void Activation();
		void Reloading();
};