#pragma once
#include <string>
#include <iostream>
#include <concepts>

class Command 
{
public:
	//TODO rule of 5
	virtual ~Command() = default;
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
