#include <A4Engine/SpritesheetComponent.hpp>
#include <A4Engine/Sprite.hpp>
#include <fmt/color.h>
#include <fmt/core.h>
#include <SDL.h> //< SDL_Rect

SpritesheetComponent::SpritesheetComponent(std::shared_ptr<const Spritesheet> spritesheet, std::shared_ptr<Sprite> targetSprite) :
m_currentAnimation(0),
m_targetSprite(std::move(targetSprite)),
m_spritesheet(std::move(spritesheet)),
m_timeAccumulator(0.f),
m_currentFrameIndex(0)
{
}

void SpritesheetComponent::PlayAnimation(const std::string& animName)
{
	auto indexOpt = m_spritesheet->GetAnimationByName(animName);
	if (!indexOpt.has_value())
	{
		fmt::print(stderr, fg(fmt::color::red), "animation \"{}\" not found\n", animName);
		return;
	}

	PlayAnimation(indexOpt.value());
}

void SpritesheetComponent::PlayAnimation(std::size_t animIndex)
{
	if (m_currentAnimation == animIndex)
		return;

	m_currentAnimation = animIndex;
	m_currentFrameIndex = 0;
	m_timeAccumulator = 0.f;
}

void SpritesheetComponent::SetColor(float r, float g, float b, float a) 
{
	m_targetSprite->SetColor(r, g, b, a);
}

void SpritesheetComponent::Update(float elapsedTime)
{
	if (m_currentAnimation >= m_spritesheet->GetAnimationCount())
		return; //< Peut arriver si aucune animation n'a été ajoutée

	const Spritesheet::Animation& anim = m_spritesheet->GetAnimation(m_currentAnimation);

	float duration = anim.frameDuration;
	if (duration >= 99)
		duration = 0.1;

	m_timeAccumulator += elapsedTime;
	while (m_timeAccumulator >= duration)
	{
		m_timeAccumulator -= duration;

		m_currentFrameIndex++;

		// On fait boucler l'animation
		if (m_currentFrameIndex >= anim.frameCount && anim.frameDuration < 99)
			m_currentFrameIndex = 0;
		else if (m_currentFrameIndex >= anim.frameCount && anim.frameDuration > 99)		//C'est terrible mais rajouter un boolean dans la struct Animation provoque des erreurs du futur
			return;																		//Du coup j'ai fait avec ce que j'avais pour pas que ca loop

		SDL_Rect rect;
		rect.x = anim.start.x + anim.size.x * m_currentFrameIndex;
		rect.y = anim.start.y;
		rect.w = anim.size.x;
		rect.h = anim.size.y;

		m_targetSprite->SetRect(rect);
	}
}
