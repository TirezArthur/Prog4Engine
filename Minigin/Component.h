#pragma once

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void FixedUpdate() {};
		virtual void Update(float elapsedsec) = 0;
		virtual void Render() const {};
		virtual void LateUpdate() {};

	protected:
		explicit Component(GameObject* m_parent) : m_parent{ m_parent } {};

		GameObject* m_parent{};
	}; 
}
