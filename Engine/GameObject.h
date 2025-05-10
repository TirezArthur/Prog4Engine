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
		requires std::constructible_from<ComponentType, GameObject*, Args...> and
				 std::derived_from<ComponentType, Component>
		ComponentType* AddComponent(Args... args);

		template<typename ComponentType>
		requires std::derived_from<ComponentType, Component>
		ComponentType* GetComponent();

		template<typename ComponentType>
		requires std::derived_from<ComponentType, Component>
		void RemoveComponent(ComponentType* component);
		
		void SetParent(GameObject* parent, bool keepWorldTransform = false);
		const std::vector<GameObject*>& GetChildren() const { return m_children; };
		bool HasChild(const GameObject* object, bool recursivelyCheckChildren = false) const;

		void SetLocalPosition(const glm::vec3& transform);
		void SetWorldPosition(const glm::vec3& transform);
		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition() const; //Conceptually const but does result in the cached global transform needing to be mutable
	private:
		void DeleteComponent(Component* component);

		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);

		void SetPositionDirty();
		mutable bool m_positionIsDirty{ true };

		glm::vec3 m_localTransform{};
		mutable glm::vec3 m_globalTransform{};

		GameObject* m_parent{};

		std::vector<std::unique_ptr<Component>> m_components{};
		std::vector<GameObject*> m_children{};
		std::vector<Component*> m_deleteQueue{};
	};

	template<typename ComponentType, typename... Args>
	requires std::constructible_from<ComponentType, GameObject*, Args...> and
			 std::derived_from<ComponentType, Component>
	inline ComponentType* GameObject::AddComponent(Args... args)
	{
		m_components.emplace_back(std::make_unique<ComponentType>(this, args...));
		return reinterpret_cast<ComponentType*>(m_components.back().get());
	}

	template<typename ComponentType>
	requires std::derived_from<ComponentType, Component>
	ComponentType* GameObject::GetComponent()
	{
		ComponentType* ret{};
		std::ignore = std::ranges::find_if(m_components, [&ret](auto& component) {
			ret = dynamic_cast<ComponentType*>(component.get());
			return ret != nullptr;
			});
		return ret;
	}

	template<typename ComponentType>
	requires std::derived_from<ComponentType, Component>
	void GameObject::RemoveComponent(ComponentType* component)
	{
		m_deleteQueue.emplace_back(static_cast<Component*>(component));
	}
}
