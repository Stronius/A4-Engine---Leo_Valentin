#pragma once

#include <A4Engine/Export.hpp>
#include <A4Engine/Matrix3.hpp>

class SDLppRenderer;
class SDLppTexture;
class Transform;

class A4ENGINE_API Renderable // interface
{
	public:
		// Il est important pour une classe virtuelle de base d'avoir un destructeur virtuel
		virtual ~Renderable() = default;

		virtual void Draw(SDLppRenderer& renderer, const Matrix3f& matrix) = 0;
};
