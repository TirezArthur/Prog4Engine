#pragma once

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void FixedUpdate() {};
		virtual void Update([[maybe_unused]] float elapsedSec) {};
		virtual void Render() const {};
		virtual void LateUpdate() {};

		virtual void SetParent(GameObject* parent) { m_parent = parent; };
	protected:
		GameObject* m_parent{};
	}; 
}
