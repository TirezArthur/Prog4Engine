#include "FpsComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

using namespace dae;

void FpsComponent::Update(float elapsedSec)
{
	if (m_textComponentTarget == nullptr) m_textComponentTarget = m_parent->GetComponent<TextComponent>();
	if (m_textComponentTarget != nullptr) m_textComponentTarget->SetText(std::to_string(static_cast<int>(1 / elapsedSec)) + " fps");
}
