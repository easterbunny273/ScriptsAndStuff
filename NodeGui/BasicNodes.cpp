#include "BasicNodes.h"

Output_Integer_Node::Output_Integer_Node(const std::string name)
	: Node(name, "IntegerOutput")
{
	m_outputParams["output"] = TParamDescription(VariableType::INTEGER);

	m_value = 0;
}

bool Output_Integer_Node::execute(EvaluationContext &context) const
{
	return true;
}

PrintInteger_Node::PrintInteger_Node(const std::string name) : Node(name, "IntegerPrint")
{
	m_inputParams["input"] = TParamDescription(VariableType::INTEGER);
}

bool PrintInteger_Node::execute(EvaluationContext &context) const
{
	return false;
}




MultipleOutTestNode::MultipleOutTestNode(const std::string name) : Node(name, "IntegerPrint")
{
	m_outputParams["output1"] = TParamDescription(VariableType::DOUBLE);
	m_outputParams["output2"] = TParamDescription(VariableType::INTEGER);
	m_outputParams["output3"] = TParamDescription(VariableType::INTEGER);
}

bool MultipleOutTestNode::execute(EvaluationContext &context) const
{
	return false;
}