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
#include "GameCommands.hpp"
#include "InputManager.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "FpsComponent.h"
#include "RotatorComponent.h"
#include "MovementComponent.h"
#include "DynamicTextComponent.h"
#include "LifeComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "Hash.hpp"
#include "ServiceLocator.h"
#include "SdlSoundManager.h"
#include <format>

using GameObject = dae::GameObject;

void load()
{
	dae::Scene* scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	dae::InputManager& inputManager = dae::InputManager::GetInstance();
	dae::ResourceManager& resourceManager = dae::ResourceManager::GetInstance();
	auto largeFont = resourceManager.LoadFont("Lingua.otf", 36);
	auto smallFont = resourceManager.LoadFont("Lingua.otf", 20);

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
	title->AddComponent<dae::TextComponent>("Programming 4 Assignment", largeFont);

	GameObject* fpsCounter = scene->Add(std::make_unique<GameObject>());
	fpsCounter->AddComponent<dae::TextComponent>("fps", largeFont);
	fpsCounter->AddComponent<dae::FpsComponent>();

	GameObject* player = scene->Add(std::make_unique<GameObject>());
	player->SetLocalPosition(glm::vec3{ 100, 100, 100 });
	player->AddComponent<dae::TextureComponent>(resourceManager.LoadTexture("Player.png"));
	player->AddComponent<dae::MovementComponent>(100.0f, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D);
	dae::LifeComponent* playerLife = player->AddComponent<dae::LifeComponent>(3);
	dae::ScoreComponent* playerScore = player->AddComponent<dae::ScoreComponent>();

	GameObject* enemy = scene->Add(std::make_unique<GameObject>());
	enemy->SetLocalPosition(glm::vec3{ 150, 100, 100 });
	enemy->AddComponent<dae::TextureComponent>(resourceManager.LoadTexture("Enemy.png"));
	enemy->AddComponent<dae::MovementComponent>(50.0f, dae::Gamepad_UP, dae::Gamepad_DOWN, dae::Gamepad_LEFT, dae::Gamepad_RIGHT);
	dae::LifeComponent* enemyLife = enemy->AddComponent<dae::LifeComponent>(3);
	dae::ScoreComponent* enemyScore = enemy->AddComponent<dae::ScoreComponent>();

	GameObject* playerLivesObject = scene->Add(std::make_unique<GameObject>());
	playerLivesObject->SetLocalPosition(glm::vec3{ 10, 60, 0 });
	playerLivesObject->AddComponent<dae::TextComponent>("#Lives: 3", smallFont);
	dae::DynamicTextComponent<int>* playerLivesDisplay = playerLivesObject->AddComponent<dae::DynamicTextComponent<int>>("#Lives: {}", dae::sdbm_hash("LivesChanged"));

	GameObject* playerScoreObject = scene->Add(std::make_unique<GameObject>());
	playerScoreObject->SetLocalPosition(glm::vec3{ 10, 80, 0 });
	playerScoreObject->AddComponent<dae::TextComponent>("Score: 0", smallFont);
	dae::DynamicTextComponent<int>* playerScoreDisplay = playerScoreObject->AddComponent<dae::DynamicTextComponent<int>>("Score: {}", dae::sdbm_hash("ScoreChanged"));

	GameObject* enemyLivesObject = scene->Add(std::make_unique<GameObject>());
	enemyLivesObject->SetLocalPosition(glm::vec3{ 10, 100, 0 });
	enemyLivesObject->AddComponent<dae::TextComponent>("#Lives: 3", smallFont);
	dae::DynamicTextComponent<int>* enemyLifeDisplay = enemyLivesObject->AddComponent<dae::DynamicTextComponent<int>>("#Lives: {}", dae::sdbm_hash("LivesChanged"));

	GameObject* enemyScoreObject = scene->Add(std::make_unique<GameObject>());
	enemyScoreObject->SetLocalPosition(glm::vec3{ 10, 120, 0 });
	enemyScoreObject->AddComponent<dae::TextComponent>("Score: 0", smallFont);
	dae::DynamicTextComponent<int>* enemyScoreDisplay = enemyScoreObject->AddComponent<dae::DynamicTextComponent<int>>("Score: {}", dae::sdbm_hash("ScoreChanged"));

	playerLife->AddObserver(dynamic_cast<dae::Observer*>(playerLivesDisplay));
	playerScore->AddObserver(dynamic_cast<dae::Observer*>(playerScoreDisplay));
	inputManager.AddBinding(SDL_SCANCODE_C, dae::TriggerType::up, std::make_unique<DamageCommand>(playerLife));
	inputManager.AddBinding(SDL_SCANCODE_X, dae::TriggerType::up, std::make_unique<ScoreCommand>(playerScore, 100));

	enemyLife->AddObserver(dynamic_cast<dae::Observer*>(enemyLifeDisplay));
	enemyScore->AddObserver(dynamic_cast<dae::Observer*>(enemyScoreDisplay));
	inputManager.AddBinding(dae::Gamepad_X, dae::TriggerType::up, std::make_unique<DamageCommand>(enemyLife));
	inputManager.AddBinding(dae::Gamepad_A, dae::TriggerType::up, std::make_unique<ScoreCommand>(enemyScore, 100));

	dae::SoundManager& SoundManager = dae::ServiceLocator::RegisterSoundManagerInstance(std::make_unique<dae::SdlSoundManager>("../Data/"));

	SoundManager.Play(dae::SoundRequest{ "shrine_activate.wav" });
	SoundManager.Play(dae::SoundRequest{ "TEST" });
	SoundManager.Play(dae::SoundRequest{ "TEST2" });
	SoundManager.Play(dae::SoundRequest{ "TEST2" });
	SoundManager.Play(dae::SoundRequest{ "TEST2" });
	SoundManager.Play(dae::SoundRequest{ "shrine_activate.wav" });
}

int main(int, char*[]) {
	dae::Engine engine("../Data/");
	engine.Run(load);
    return 0;
}