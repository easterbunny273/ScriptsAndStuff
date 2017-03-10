#pragma once

#include <string>
#include <vector>
#include <map>

#include "IVariable.h"
#include "EvaluationContext.h"

class Node
{
public:
	Node(const std::string &name, const std::string &type) : m_name(name), m_type(type) {};

	using TParamName = std::string;

	struct TParamDescription
	{
		TParamDescription() : type(VariableType::UNDEFINED) {};
		TParamDescription(VariableType _type) : type(_type) {};

		VariableType		type;

		bool operator==(const TParamDescription &other) { return (type == other.type); }
	};

	using InputParamDescription = std::map<TParamName, TParamDescription>;
	using OutputParamDescription = std::map<TParamName, TParamDescription>;

	const InputParamDescription& getInputParamDescription() const;
	const OutputParamDescription& getOuputParamDescription() const;

	//bool canEvaluate(EvaluationContext &context) const;
	virtual bool execute(EvaluationContext &context) const = 0;

	auto setName(const std::string name) { m_name = name; }
	auto getName() const { return m_name; }
	auto getType() const { return m_type; }
	
protected:
	InputParamDescription	m_inputParams;
	OutputParamDescription	m_outputParams;

private:
	std::string m_name;
	const std::string m_type;
};