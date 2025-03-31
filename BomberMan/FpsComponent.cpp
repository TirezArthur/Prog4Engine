#include "FpsComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <numeric>
#include <format>

using namespace dae;

FpsComponent::FpsComponent(GameObject* parent) : Component{ parent }
{
	assert(m_Parent->GetComponent<TextComponent>() != nullptr);
}

void FpsComponent::Update(float elapsedSec)
{
	if (m_textComponentTarget == nullptr) m_textComponentTarget = m_Parent->GetComponent<TextComponent>();

	m_elapsedValues.push_back(elapsedSec);
	if (m_elapsedValues.size() > MAXLISTSIZE) m_elapsedValues.pop_front();

	if (m_textComponentTarget == nullptr) return;

	float elapsedSecAvg = std::accumulate(m_elapsedValues.begin(), m_elapsedValues.end(), 0.0f) / m_elapsedValues.size();
	m_textComponentTarget->SetText(std::format("{} fps", static_cast<int>(1.0f / elapsedSecAvg)));
}
