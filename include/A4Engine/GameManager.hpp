#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Vector2.hpp>


class A4ENGINE_API GameManager
{
	public:
		GameManager();
		GameManager(const GameManager&) = delete;
		GameManager(GameManager&&) = delete;
		~GameManager();

		static GameManager& Instance();


	private:

	static GameManager* my_instance;
};
