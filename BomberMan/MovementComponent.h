#pragma once
#include "Component.h"
#include "InputManager.h"
#include <array>

namespace dae
{
	class MovementComponent : public Component
	{
	public:
		template<typename ButtonType>
		explicit MovementComponent(GameObject* parent, float velocity, ButtonType up, ButtonType down, ButtonType left, ButtonType right);
		virtual ~MovementComponent();
		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;

		void Update(float elapsedSec) override;
	private:
		float m_Velocity{};
		struct Direction {
			int x;
			int y;
		} m_TargetDirection{};
		std::array<Command*, 8> m_RegisteredCommands{};
	};

	template<typename ButtonType>
	inline MovementComponent::MovementComponent(GameObject* parent, float velocity, ButtonType up, ButtonType down, ButtonType left, ButtonType right) : Component{ parent },
		m_Velocity{ velocity }
	{
		dae::InputManager& inputManager = InputManager::GetInstance();
		m_RegisteredCommands[0] = inputManager.AddBinding(up, TriggerType::down, std::make_unique<ValueModifierCommand<int>>(m_TargetDirection.y, 1));
		m_RegisteredCommands[1] = inputManager.AddBinding(up, TriggerType::up, std::make_unique<ValueModifierCommand<int>>(m_TargetDirection.y, -1));
		m_RegisteredCommands[2] = inputManager.AddBinding(down, TriggerType::down, std::make_unique<ValueModifierCommand<int>>(m_TargetDirection.y, -1));
		m_RegisteredCommands[3] = inputManager.AddBinding(down, TriggerType::up, std::make_unique<ValueModifierCommand<int>>(m_TargetDirection.y, 1));
		m_RegisteredCommands[4] = inputManager.AddBinding(right, TriggerType::down, std::make_unique<ValueModifierCommand<int>>(m_TargetDirection.x, 1));
		m_RegisteredCommands[5] = inputManager.AddBinding(right, TriggerType::up, std::make_unique<ValueModifierCommand<int>>(m_TargetDirection.x, -1));
		m_RegisteredCommands[6] = inputManager.AddBinding(left, TriggerType::down, std::make_unique<ValueModifierCommand<int>>(m_TargetDirection.x, -1));
		m_RegisteredCommands[7] = inputManager.AddBinding(left, TriggerType::up, std::make_unique<ValueModifierCommand<int>>(m_TargetDirection.x, 1));
	}
}

