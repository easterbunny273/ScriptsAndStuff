#pragma once

#ifndef __VALUEPOOL_H_
#define __VALUEPOOL_H_

#include "INode.h"
#include "IArgument.h"
#include "Values.h"

#include <memory>
#include <unordered_map>
#include <map>
#include <string>

class ValuePool
{
public:
	std::shared_ptr<IValue> getValue(PtrNode node, const std::string & outputArgumentName, TArgumentType type)
	{
		// build key
		TMapKey key(std::make_pair(node, outputArgumentName));

		// try to get it from cache
		auto valueIter = m_valueMap.find(key);
		bool hasValue = valueIter != m_valueMap.end() && valueIter->second;

		// return found value or create new value
		std::shared_ptr<IValue> returnValue(hasValue ? valueIter->second : createValueForType(type));
		assert(returnValue->get_type() == type);

		// if new, store it in map
		if (!hasValue)
			m_valueMap[key] = returnValue;

		return returnValue;
	}

	void clear()
	{
		m_valueMap.clear();
	}

private:
	using TMapKey = std::pair<PtrNode, std::string>;
	using TMapValue = std::shared_ptr<IValue>;
	using TMap = std::map<TMapKey, TMapValue>;

	TMap	m_valueMap;
};

#endif 