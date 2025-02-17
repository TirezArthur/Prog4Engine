#pragma once
#include "Component.h"

namespace dae
{
	class PeriodicMovementComponent final : public Component
	{
	public:
		explicit PeriodicMovementComponent(GameObject* parent, float scaleX, float scaleY, float periodX, float periodY);
		virtual ~PeriodicMovementComponent() = default;
		PeriodicMovementComponent(const PeriodicMovementComponent& other) = delete;
		PeriodicMovementComponent(PeriodicMovementComponent&& other) = delete;
		PeriodicMovementComponent& operator=(const PeriodicMovementComponent& other) = delete;
		PeriodicMovementComponent& operator=(PeriodicMovementComponent&& other) = delete;

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

