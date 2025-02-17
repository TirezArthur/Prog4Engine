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

void GameObject::LateUpdate(float elapsedSec)
{
	for (const auto& component : m_components) {
		component->LateUpdate(elapsedSec);
	}
	for (Component* component : m_deleteQueue) {
		DeleteComponent(component);
	}
	m_deleteQueue.clear();
}

void GameObject::Render() const
{
	for (const auto& component : m_components) {
		component->Render();
	}
}

void GameObject::SetParent(GameObject* parent, bool keepWorldTransform)
{
	assert(parent != this && parent != m_parent);
	if (parent == this || parent == m_parent) return;

	if (parent == nullptr) {
		SetLocalPosition(GetWorldPosition());
	}
	else {
		if (keepWorldTransform) SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		m_positionIsDirty = true;
	}

	if (m_parent) m_parent->RemoveChild(this);
	m_parent = parent;
	if (m_parent) m_parent->AddChild(this);
}

void GameObject::SetLocalPosition(const glm::vec3& transform)
{
	m_localTransform = transform;
	m_positionIsDirty = true;
}

void GameObject::SetWorldPosition(const glm::vec3& transform)
{
	SetLocalPosition((!m_parent) ? transform :m_parent->GetWorldPosition() - transform);
}

const glm::vec3& GameObject::GetLocalPosition() const
{
	return m_localTransform;
}

const glm::vec3& GameObject::GetWorldPosition()
{
	if (m_positionIsDirty) {
		m_globalTransform = (!m_parent) ? GetLocalPosition() : GetLocalPosition() + m_parent->GetWorldPosition();
		m_positionIsDirty = false;
	}
	return m_globalTransform;
}

void GameObject::DeleteComponent(Component* component)
{
	std::erase_if(m_components, [component](const std::unique_ptr<Component>& ownedComponent) {
		return ownedComponent.get() == component;
		});
}

void GameObject::AddChild(GameObject* child)
{
	m_children.emplace_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	std::erase(m_children, child);
}
