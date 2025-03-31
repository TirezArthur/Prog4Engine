#include "MovementComponent.h"
#include "GameObject.h"

using namespace dae;

MovementComponent::~MovementComponent()
{
	//InputManager& inputManager = InputManager::GetInstance();
	//for (Command* command : m_RegisteredCommands) inputManager.RemoveBinding(command);
}

void MovementComponent::Update(float elapsedSec)
{
	glm::vec3 position = m_Parent->GetLocalPosition();
	position.x += m_TargetDirection.x * elapsedSec * m_Velocity;
	position.y -= m_TargetDirection.y * elapsedSec * m_Velocity;
	m_Parent->SetLocalPosition(position);
}
