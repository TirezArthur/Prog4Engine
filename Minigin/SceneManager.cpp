#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

SceneManager::~SceneManager() = default;

SceneManager::SceneManager() = default;

void SceneManager::FixedUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}

void SceneManager::Update(float elapsedSec)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(elapsedSec);
	}
}

void SceneManager::Render() const
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::LateUpdate(float elapsedSec)
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate(elapsedSec);
	}
}

Scene* SceneManager::CreateScene(const std::string& name)
{
	m_scenes.emplace_back(new Scene{ name });
	return m_scenes.back().get();
}
