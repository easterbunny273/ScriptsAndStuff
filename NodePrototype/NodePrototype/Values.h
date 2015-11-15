#pragma once

#ifndef __VALUES_H_
#define __VALUES_H_

#include <string>
#include <cassert>

#include "IArgument.h"

class IValue
{
public:
	IValue(TArgumentType type) : m_type(type) {};
	virtual ~IValue() {};

	TArgumentType get_type() const { return m_type; };

protected:
	static TArgumentType getTypeForTemplate(double) { return TArgumentType::DOUBLE; }
	static TArgumentType getTypeForTemplate(int) { return TArgumentType::INTEGER; }
	static TArgumentType getTypeForTemplate(std::string &) { return TArgumentType::STRING; }
	static TArgumentType getTypeForTemplate(bool) { return TArgumentType::BOOLEAN; }

private:
	TArgumentType m_type;
};

using IValueVector = std::vector<IValue *>;
using ConstIValueVector = std::vector<const IValue *>;

template <class T> class ConcreteValue : public IValue 
{
public:
	ConcreteValue() : IValue(IValue::getTypeForTemplate(T())) {};
	ConcreteValue(T initValue) : ConcreteValue(), m_value(initValue) {};

	void set(T value) { m_value = value; }
	T get() const { return m_value; }

private:
	T m_value;
};

static std::unique_ptr<IValue> createValueForType(TArgumentType type)
{
	std::unique_ptr<IValue> returnValue;

	switch (type)
	{
	case TArgumentType::BOOLEAN:
		returnValue.reset(new ConcreteValue<bool>());
		break;
	case TArgumentType::INTEGER:
		returnValue.reset(new ConcreteValue<int>());
		break;
	case TArgumentType::STRING:
		returnValue.reset(new ConcreteValue<std::string>());
		break;
	case TArgumentType::DOUBLE:
		returnValue.reset(new ConcreteValue<double>());
		break;
	default:
		assert(!"unknown type");
	}	

	return returnValue;
}

#endif