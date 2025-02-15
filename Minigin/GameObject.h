#pragma once
#include "Transform.h"
#include "Component.h"
#include <memory>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace dae
{
	class GameObject final
	{
	public:
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void FixedUpdate();
		void Update(float elapsedSec);
		void LateUpdate();
		void Render() const;

		template<typename ComponentType>
		requires std::derived_from<ComponentType, Component>
		Component* AddComponent(std::unique_ptr<ComponentType> component);

		template<typename ComponentType>
		requires std::derived_from<ComponentType, Component>
		ComponentType* GetComponent();

		template<typename ComponentType>
		requires std::derived_from<ComponentType, Component>
		void RemoveComponent(ComponentType* component);
		
		void SetPosition(float x, float y);
		const Transform& GetTransform() const;
	private:
		void DeleteComponent(Component* component);

		Transform m_transform{};

		std::vector<std::unique_ptr<Component>> m_components{};
		std::vector<Component*> m_deleteQueue{};
	};

	template<typename ComponentType>
	requires std::derived_from<ComponentType, Component>
	inline Component* GameObject::AddComponent(std::unique_ptr<ComponentType> component)
	{
		m_components.emplace_back(std::move(component));
		m_components.back()->SetParent(this);
		return m_components.back().get();
	}

	template<typename ComponentType>
	requires std::derived_from<ComponentType, Component>
	inline ComponentType* GameObject::GetComponent()
	{
		ComponentType* ret{};
		std::ignore = std::find_if(m_components.begin(), m_components.end(), [&ret](auto& component) {
			ret = dynamic_cast<ComponentType*>(component.get());
			return ret != nullptr;
			});
		return ret;
	}

	template<typename ComponentType>
	requires std::derived_from<ComponentType, Component>
	inline void GameObject::RemoveComponent(ComponentType* component)
	{
		m_deleteQueue.emplace_back(static_cast<Component*>(component));
	}
}
