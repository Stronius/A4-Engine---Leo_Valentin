#include <AL/al.h>
#include <AL/alc.h>
#define DR_WAV_IMPLEMENTATION
#include <A4Engine/CameraComponent.hpp>
#include <A4Engine/GraphicsComponent.hpp>
#include <A4Engine/InputManager.hpp>
#include <A4Engine/ResourceManager.hpp>
#include <A4Engine/Sprite.hpp>
#include <A4Engine/Transform.hpp>
#include <A4Engine/SDLpp.hpp>
#include <A4Engine/SDLppRenderer.hpp>
#include <A4Engine/SDLppWindow.hpp>
#include <dr_wav.h>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	const char* deviceList = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
	std::vector<std::string> devices;
	while (true)
	{
		std::size_t length = std::strlen(deviceList);
		if (length == 0)
			break;

		devices.emplace_back(deviceList, length);

		deviceList += length + 1;
	}

	ALCdevice* device = alcOpenDevice(nullptr);

	ALCcontext* context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);

	// On va faire des trucs !
	drwav wav;
	if (!drwav_init_file(&wav, "assets/siren.wav", nullptr))
	{
		std::cout << "failed to load file" << std::endl;
		return 0;
	}

	std::vector<std::int16_t> samples(wav.totalPCMFrameCount * wav.channels);
	drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, samples.data());

	ALuint buffer;
	alGenBuffers(1, &buffer);

	alBufferData(buffer, 
		(wav.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, 
		samples.data(), 
		samples.size() * sizeof(std::int16_t),
		wav.sampleRate);

	drwav_uninit(&wav);

	ALuint source;
	alGenSources(1, &source);

	alSourcei(source, AL_BUFFER, buffer);
	alSourcei(source, AL_LOOPING, AL_TRUE);

	alListener3f(AL_POSITION, 640.f / 100.f, 360.f / 100.f, 0.f);

	alSourcePlay(source);

	SDLpp sdl;
	SDLppWindow window("Test audio", 1280, 720);
	SDLppRenderer renderer(window, "", SDL_RENDERER_PRESENTVSYNC);

	ResourceManager resourceManager(renderer);
	InputManager inputManager;

	inputManager.BindKeyPressed(SDLK_LEFT, "MoveLeft");
	inputManager.BindKeyPressed(SDLK_RIGHT, "MoveRight");

	Sprite ambulance(resourceManager.GetTexture("assets/ambulance.png"));
	ambulance.Resize(ambulance.GetWidth() * 0.1f, ambulance.GetHeight() * 0.1f);
	ambulance.SetOrigin(Vector2f(0.5f, 0.5f));

	Transform cameraTransform;
	Transform ambulanceTransform;
	ambulanceTransform.SetPosition(Vector2f(640, 360));

	Uint64 lastUpdate = SDL_GetPerformanceCounter();

	bool isOpen = true;
	while (isOpen)
	{
		Uint64 now = SDL_GetPerformanceCounter();
		float deltaTime = (float)(now - lastUpdate) / SDL_GetPerformanceFrequency();
		lastUpdate = now;

		SDL_Event event;
		while (sdl.PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				isOpen = false;

			inputManager.HandleEvent(event);
		}
		
		renderer.SetDrawColor(0, 127, 255, 255);
		renderer.Clear();

		Vector2f oldPosition = ambulanceTransform.GetGlobalPosition();

		if (inputManager.IsActive("MoveLeft"))
			ambulanceTransform.Translate(Vector2f(-1000.f * deltaTime, 0.f));

		if (inputManager.IsActive("MoveRight"))
			ambulanceTransform.Translate(Vector2f(1000.f * deltaTime, 0.f));

		Vector2f ambulancePos = ambulanceTransform.GetGlobalPosition();
		Vector2f velocity = (ambulancePos - oldPosition) / deltaTime;

		alSource3f(source, AL_POSITION, ambulancePos.x / 100.f, ambulancePos.y / 100.f, 0.f);
		alSource3f(source, AL_VELOCITY, velocity.x / 100.f, velocity.y / 100.f, 0.f);

		ambulance.Draw(renderer, cameraTransform, ambulanceTransform);

		renderer.Present();
	}

	// Libération
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);

	alcCloseDevice(device);
}