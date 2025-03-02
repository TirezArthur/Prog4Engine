#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

GameObject* Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
	return m_objects.back().get();
}

void Scene::Remove(GameObject* object)
{
	m_deleteQueue.emplace_back(object);
	const auto& children{ object->GetChildren() };
	for (GameObject* child : children) {
		Remove(child);
	}
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update(float elapsedSec)
{
	for(auto& object : m_objects)
	{
		object->Update(elapsedSec);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void dae::Scene::LateUpdate(float elapsedSec)
{
	for (const auto& object : m_objects)
	{
		object->LateUpdate(elapsedSec);
	}
	for (GameObject* object : m_deleteQueue) 
	{
		DeleteObject(object);
	}
	m_deleteQueue.clear();
}

void dae::Scene::DeleteObject(GameObject* object)
{
	std::erase_if(m_objects, [object](const std::unique_ptr<GameObject>& ownedObject) {
		return ownedObject.get() == object;
		});
}
