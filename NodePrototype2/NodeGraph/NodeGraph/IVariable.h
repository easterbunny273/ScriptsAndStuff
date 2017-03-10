#pragma once

#include <typeinfo>
#include <memory>

enum class VariableType
{
	UNDEFINED,

	INTEGER,
	DOUBLE,
	STRING,
	MATRIX_4_4,
};


class IVariable
{
public:
	IVariable(size_t hashCode) : m_hashCode(hashCode) {};
	virtual ~IVariable() {};

	size_t hashCode() const { return m_hashCode; }

	std::shared_ptr<IVariable*> createVariable(VariableType type);
private:
	const size_t m_hashCode;
};

template <class T> class Variable : public IVariable
{
public:
	Variable() : IVariable(StaticHashCode()) {};

	T value;

	static size_t StaticHashCode() { return typeid(T).hash_code(); }
};