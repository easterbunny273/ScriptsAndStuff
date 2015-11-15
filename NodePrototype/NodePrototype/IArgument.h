#pragma once

#ifndef __IARGUMENT_H_
#define __IARGUMENT_H_

#include <memory>
#include <string>

enum class TArgumentType
{
	INTEGER,
	DOUBLE,
	STRING,
	BOOLEAN
};


class IArgument
{
public:
	static std::unique_ptr<IArgument> createArgument(const std::string& name, TArgumentType type)
	{
		std::unique_ptr<IArgument> returnValue;
		returnValue.reset(new IArgument(name, type));
		return returnValue;
	}

	TArgumentType get_type() const { return m_type; };
	const char * get_name() const { return m_name.c_str(); };

private:
	IArgument(const std::string& name, TArgumentType type) : m_name(name), m_type(type) {};

	std::string m_name;
	TArgumentType m_type;
};

#endif