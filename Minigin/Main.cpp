#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "FpsComponent.h"
#include "RotatorComponent.h"
#include "MovementComponent.h"
#include "InputManager.h"
//#include "CacheTestComponent.h"
#include "GameObject.h"
#include "Scene.h"

using GameObject = dae::GameObject;

void load()
{
	dae::Scene* scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	dae::ResourceManager& resourceManager = dae::ResourceManager::GetInstance();
	auto font = resourceManager.LoadFont("Lingua.otf", 36);

	GameObject* background = scene->Add(std::make_unique<GameObject>());
	background->SetLocalPosition(glm::vec3{ 0, 0, 0 });
	background->AddComponent<dae::TextureComponent>(resourceManager.LoadTexture("background.tga"));
	
	GameObject* logoCenter = scene->Add(std::make_unique<GameObject>());
	logoCenter->SetLocalPosition(glm::vec3{ 216, 180, 0 });

	GameObject* logoRotatingCenter = scene->Add(std::make_unique<GameObject>());
	logoRotatingCenter->SetLocalPosition(glm::vec3{ 216, 180, 0 });
	logoRotatingCenter->AddComponent<dae::RotatorComponent>(120.0f, 0.0f, 5.0f, 5.0f);
	logoRotatingCenter->SetParent(logoCenter);

	GameObject* logo = scene->Add(std::make_unique<GameObject>());
	logo->SetLocalPosition(glm::vec3{ 216, 180, 0 });
	logo->AddComponent<dae::TextureComponent>(resourceManager.LoadTexture("logo.tga"));
	logo->AddComponent<dae::RotatorComponent>(20.f, 20.f, 1.0f, 1.0f);
	logo->SetParent(logoRotatingCenter);

	GameObject* title = scene->Add(std::make_unique<GameObject>());
	title->SetLocalPosition(glm::vec3{ 80, 20, 0 });
	title->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);

	GameObject* fpsCounter = scene->Add(std::make_unique<GameObject>());
	fpsCounter->AddComponent<dae::TextComponent>("fps", font);
	fpsCounter->AddComponent<dae::FpsComponent>();

	GameObject* player = scene->Add(std::make_unique<GameObject>());
	player->SetLocalPosition(glm::vec3{ 100, 100, 100 });
	player->AddComponent<dae::TextureComponent>(resourceManager.LoadTexture("Player.png"));
	player->AddComponent<dae::MovementComponent>(100.0f, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D);

	GameObject* enemy = scene->Add(std::make_unique<GameObject>());
	enemy->SetLocalPosition(glm::vec3{ 150, 100, 100 });
	enemy->AddComponent<dae::TextureComponent>(resourceManager.LoadTexture("Enemy.png"));
	enemy->AddComponent<dae::MovementComponent>(50.0f, dae::GamepadButton::Gamepad_UP, dae::GamepadButton::Gamepad_DOWN, dae::GamepadButton::Gamepad_LEFT, dae::GamepadButton::Gamepad_RIGHT);

	//GameObject* cacheTest = scene->Add(std::make_unique<GameObject>());
	//cacheTest->AddComponent<dae::CacheTestComponent>();
}

int main(int, char*[]) {
	dae::Engine engine("../Data/");
	engine.Run(load);
    return 0;
}