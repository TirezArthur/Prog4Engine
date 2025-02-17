#define _USE_MATH_DEFINES
#include "PeriodicMovementComponent.h"
#include "GameObject.h"
#include <cmath>

using namespace dae;

PeriodicMovementComponent::PeriodicMovementComponent(GameObject* parent, float scaleX, float scaleY, float periodX, float periodY) : Component{ parent },
m_periodX{periodX},
m_periodY{periodY},
m_scaleX{scaleX},
m_scaleY{scaleY}
{
}

void PeriodicMovementComponent::Update(float elapsedSec)
{
	m_elapsedX += elapsedSec;
	m_elapsedY += elapsedSec;
	while (m_elapsedX > m_periodX) m_elapsedX -= m_periodX;
	while (m_elapsedY > m_periodY) m_elapsedY -= m_periodY;
	m_parent->SetLocalPosition(glm::vec3{ m_scaleX * cos(2.0f * M_PI * m_elapsedX / m_periodX), m_scaleY * sin(2.0f * M_PI * m_elapsedY / m_periodY), 0.0f });
}
