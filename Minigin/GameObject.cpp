#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

using namespace dae;

GameObject::~GameObject() = default;

void GameObject::FixedUpdate()
{
	for (const auto& component : m_components) {
		component->FixedUpdate();
	}
}

void GameObject::Update(float elapsedSec)
{
	for (const auto& component : m_components) {
		component->Update(elapsedSec);
	}
}

void GameObject::Render() const
{
	for (const auto& component : m_components) {
		component->Render();
	}
}

void GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

const Transform& GameObject::GetTransform() const
{
	return m_transform;
}
