#include "MovementComponent.h"
#include "GameObject.h"

using namespace dae;

dae::MovementComponent::~MovementComponent()
{
	//dae::InputManager& inputManager = InputManager::GetInstance();
	//for (Command* command : m_RegisteredCommands) inputManager.RemoveBinding(command);
}

void dae::MovementComponent::Update(float elapsedSec)
{
	glm::vec3 position = m_parent->GetLocalPosition();
	position.x += m_TargetDirection.x * elapsedSec * m_Velocity;
	position.y -= m_TargetDirection.y * elapsedSec * m_Velocity;
	m_parent->SetLocalPosition(position);
}
