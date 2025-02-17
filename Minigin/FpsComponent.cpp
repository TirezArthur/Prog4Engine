#include "FpsComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <numeric>
#include <format>

using namespace dae;

dae::FpsComponent::FpsComponent(GameObject* parent) : Component{ parent }
{
}

void FpsComponent::Update(float elapsedSec)
{
	if (m_textComponentTarget == nullptr) m_textComponentTarget = m_parent->GetComponent<TextComponent>();
	//This breaks if the text component gets deleted but getting component every frame is also slow
	//TODO maybe make oberserver pattern, no clue how yet though

	m_elapsedValues.push_back(elapsedSec);
	if (m_elapsedValues.size() > MAXLISTSIZE) m_elapsedValues.pop_front();

	if (m_textComponentTarget == nullptr) return;

	float elapsedSecAvg = std::accumulate(m_elapsedValues.begin(), m_elapsedValues.end(), 0.0f) / m_elapsedValues.size();
	m_textComponentTarget->SetText(std::format("{} fps", static_cast<int>(1.0f / elapsedSecAvg)));
}
