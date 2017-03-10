#pragma once

#include "Node.h"

class Output_Integer_Node : public Node
{
public:
	Output_Integer_Node(const std::string name);

	void setValue(int value) { m_value = value; }
	auto getValue() const { return m_value; }

	virtual bool execute(EvaluationContext &context) const;

private:
	int m_value;
};

class PrintInteger_Node : public Node
{
public:
	PrintInteger_Node(const std::string name);

	virtual bool execute(EvaluationContext &context) const;

private:
	int m_value;
};

class MultipleOutTestNode : public Node
{
public:
	MultipleOutTestNode(const std::string name);

	virtual bool execute(EvaluationContext &context) const;

private:
	int m_value;
};