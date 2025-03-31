#pragma once
#include "Command.hpp"
#include "LifeComponent.h"
#include "ScoreComponent.h"

class DamageCommand : public Command
{
public:
	DamageCommand(dae::LifeComponent* lifeTarget) : m_LifeComponent{ lifeTarget } {}
	virtual void Execute() override { m_LifeComponent->LoseLife(); };
private:
	dae::LifeComponent* m_LifeComponent;
};

class ScoreCommand : public Command
{
public:
	ScoreCommand(dae::ScoreComponent* scoreTarget, int score) : m_ScoreComponent{ scoreTarget }, m_Score{ score } {}
	virtual void Execute() override { m_ScoreComponent->IncrementScore(m_Score); };
private:
	dae::ScoreComponent* m_ScoreComponent;
	int m_Score;
};
