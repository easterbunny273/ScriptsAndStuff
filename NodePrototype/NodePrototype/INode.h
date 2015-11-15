#pragma once

#ifndef __INODE_H_
#define __INODE_H_

#include <memory>
#include <vector>

#include "Values.h"

class IArgument;
class Binding;

class INode
{
public:
	using PtrIArgument = std::shared_ptr<IArgument>;
	using ArgumentVector = std::vector<PtrIArgument>;

	INode(const ArgumentVector& inputArguments, const ArgumentVector& outputArguments) : m_inputArguments(inputArguments), m_outputArguments(outputArguments) {};
	const ArgumentVector& get_input_arguments() const { return m_inputArguments; };
	const ArgumentVector& get_output_arguments() const { return m_outputArguments; };

	virtual bool execute(const ConstIValueVector& inputValues, const IValueVector& outputValues) = 0;

private:
	ArgumentVector m_inputArguments, m_outputArguments;
};

using PtrNode = std::shared_ptr<INode>;

#endif