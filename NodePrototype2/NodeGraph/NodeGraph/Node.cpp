#include "Node.h"

const Node::InputParamDescription& Node::getInputParamDescription() const
{
	return m_inputParams;
}

const Node::OutputParamDescription& Node::getOuputParamDescription() const
{
	return m_outputParams;
}
