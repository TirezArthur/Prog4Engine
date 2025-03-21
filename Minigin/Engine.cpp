#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <thread>
#include <iostream>
#include "Engine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Engine::Engine(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	SDL_DisplayMode displayMode{};
	SDL_GetWindowDisplayMode(g_window, &displayMode);

	m_FrameTimetep = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<float>(1.0f / displayMode.refresh_rate)); // Convert refresh rate to microseconds
}

dae::Engine::~Engine()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Engine::SetFixedTimeStep(float fixedTimeStep)
{
	m_FixedTimeStep = fixedTimeStep;
}

void dae::Engine::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	bool running{ true };
	auto lastFrame{ std::chrono::high_resolution_clock::now() };
	float totalElapsedSec{ 0.0f };
	while (running)
	{
		const auto currentFrame{ std::chrono::high_resolution_clock::now() };
		const auto elapsedSec{ std::chrono::duration<float>(currentFrame - lastFrame).count() };
		lastFrame = currentFrame;
		totalElapsedSec += elapsedSec;

		running = input.ProcessInput();

		while (totalElapsedSec > m_FixedTimeStep)
		{
			sceneManager.FixedUpdate();
			totalElapsedSec -= m_FixedTimeStep;
		}

		sceneManager.Update(elapsedSec);

		sceneManager.LateUpdate(elapsedSec);

		renderer.Render();

		const auto currentTime{ std::chrono::high_resolution_clock::now() };
		const auto sleepTime{ currentFrame - currentTime + m_FrameTimetep };
		std::this_thread::sleep_for(sleepTime);
	}
}
