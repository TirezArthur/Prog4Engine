#pragma once
#include "LifeComponent.h"
#include "ScoreComponent.h"
#include <string>
#include <iostream>
#include <concepts>

class Command 
{
public:
	explicit Command() = default;
	virtual ~Command() = default;
	Command(const Command& other) = delete;
	Command(Command&& other) = delete;
	Command& operator=(const Command& other) = delete;
	Command& operator=(Command&& other) = delete;

	virtual void Execute() = 0;
};

class DebugPrintCommand : public Command 
{
public:
	DebugPrintCommand(const std::string& string) : m_String{ string } {}
	virtual void Execute() override { std::cout << m_String << std::endl; };
private:
	std::string m_String;
};

template<typename ValueType>
requires std::is_integral_v<ValueType> 
or std::is_floating_point_v<ValueType>
class ValueModifierCommand : public Command
{
public:
	ValueModifierCommand(ValueType& value, ValueType modifier) : m_Value{ value }, m_Modifier{ modifier } {}
	virtual void Execute() override { m_Value += m_Modifier; };
private:
	ValueType& m_Value;
	ValueType m_Modifier;
};

//TODO bad, should be generic
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
