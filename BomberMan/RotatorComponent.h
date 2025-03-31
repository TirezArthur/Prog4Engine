#pragma once
#include "Component.h"

namespace dae
{
	class RotatorComponent final : public Component
	{
	public:
		explicit RotatorComponent(GameObject* parent, float scaleX, float scaleY, float periodX, float periodY);
		virtual ~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		virtual void Update(float elapsedSec) override;

	private:
		float m_elapsedX{};
		float m_elapsedY{};
		float m_periodX{};
		float m_periodY{};
		float m_scaleX{};
		float m_scaleY{};
	};
}

