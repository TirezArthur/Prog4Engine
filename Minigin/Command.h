#pragma once
#include <string>
#include <iostream>

class Command 
{
public:
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
