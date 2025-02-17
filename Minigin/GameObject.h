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
		void LateUpdate(float elapsedSec);
		void Render() const;

		template<typename ComponentType, typename... Args>
		requires std::constructible_from<ComponentType, GameObject*, Args...>
		Component* AddComponent(Args... args);

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

	template<typename ComponentType, typename... Args>
	requires std::constructible_from<ComponentType, GameObject*, Args...>
	inline Component* GameObject::AddComponent(Args... args)
	{
		m_components.emplace_back(std::make_unique<ComponentType>(this, args...));
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
