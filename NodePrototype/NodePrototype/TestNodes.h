#pragma once

#ifndef __testnodes_h_

#include "INode.h"
#include "Values.h"

#include <Windows.h>

class ConstIntNode : public INode 
{
public:
	ConstIntNode(int value) : INode(ArgumentVector(), ArgumentVector({ IArgument::createArgument("result", TArgumentType::INTEGER) })), m_value(value) {};

	virtual bool execute(const ConstIValueVector& inputValues, const IValueVector& outputValues)
	{
		bool correctParams = (outputValues.size() == 1 && outputValues[0]->get_type() == TArgumentType::INTEGER);
		assert(correctParams);
		
		if (correctParams)
		{
			auto outputValue = dynamic_cast<ConcreteValue<int>*>(outputValues[0]);
			assert(outputValue);

			outputValue->set(m_value);
		}

		return correctParams;
	}

private:
	const int m_value;
};

class AddIntNode : public INode
{
public:
	AddIntNode() : INode(ArgumentVector({ IArgument::createArgument("lhs", TArgumentType::INTEGER), 
										  IArgument::createArgument("rhs", TArgumentType::INTEGER) }), 
						ArgumentVector({ IArgument::createArgument("result", TArgumentType::INTEGER) })) {};

	virtual bool execute(const ConstIValueVector& inputValues, const IValueVector& outputValues)
	{
		bool correctParams = (outputValues.size() == 1 && outputValues[0]->get_type() == TArgumentType::INTEGER && 
							  inputValues.size() == 2 && inputValues[0]->get_type() == TArgumentType::INTEGER &&
								inputValues[1]->get_type() == TArgumentType::INTEGER);
		assert(correctParams);

		if (correctParams)
		{
			auto inputValue1 = dynamic_cast<const ConcreteValue<int>*>(inputValues[0]);
			assert(inputValue1);

			auto inputValue2 = dynamic_cast<const ConcreteValue<int>*>(inputValues[1]);
			assert(inputValue2);

			auto outputValue = dynamic_cast<ConcreteValue<int>*>(outputValues[0]);
			assert(outputValue);

			Sleep(3000);

			outputValue->set(inputValue1->get() + inputValue2->get());
		}

		return correctParams;
	}
};

class IntOutputNode : public INode
{
public:
	IntOutputNode(const std::string & identifier) : INode(ArgumentVector({ IArgument::createArgument("value", TArgumentType::INTEGER) }), ArgumentVector()), m_identifier(identifier) {};

	virtual bool execute(const ConstIValueVector& inputValues, const IValueVector& outputValues)
	{
		bool correctParams = (inputValues.size() == 1 && inputValues.at(0)->get_type() == TArgumentType::INTEGER);
		assert(correctParams);

		if (correctParams)
		{
			auto inputValue = dynamic_cast<const ConcreteValue<int>*>(inputValues[0]);
			assert(inputValue);

			std::cout << m_identifier << " got input: " << inputValue->get() << std::endl;
		}

		return correctParams;
	}

private:
	std::string m_identifier;
};

#endif