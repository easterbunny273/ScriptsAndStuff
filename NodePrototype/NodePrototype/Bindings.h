#pragma once

#ifndef __BINDINGS_H_
#define __BINDINGS_H_

#include <memory>

#include "IArgument.h"
#include "INode.h"

class Binding;

using PtrBinding = std::shared_ptr<Binding>;

class Binding
{
public:
	
	static PtrBinding createBinding(PtrNode source, const char * sourceArgumentName,
		PtrNode target, const char * targetArgumentName)
	{
		auto sourceOutputArguments = source->get_output_arguments();
		auto targetInputArguments = target->get_input_arguments();

		std::shared_ptr<IArgument> sourceArgument, targetArgument;

		if (!sourceArgumentName && sourceOutputArguments.size()== 1)
			sourceArgumentName = sourceOutputArguments[0]->get_name();

		if (!targetArgumentName && targetInputArguments.size() == 1)
			targetArgumentName = targetInputArguments[0]->get_name();

		for (auto iter : sourceOutputArguments)
		{
			if (sourceArgumentName && strcmp(iter->get_name(), sourceArgumentName) == 0)
				sourceArgument = iter;
		}

		for (auto iter : targetInputArguments)
		{
			if (targetArgumentName && strcmp(iter->get_name(), targetArgumentName) == 0)
				targetArgument = iter;
		}
		

		if (!sourceArgument || !targetArgument)
			return nullptr;

		if (sourceArgument->get_type() != targetArgument->get_type())
			return nullptr;

		return PtrBinding(new Binding(sourceArgument->get_type(), source, sourceArgumentName, target, targetArgumentName));
	}

	static PtrBinding createBinding(PtrNode source, PtrNode target) { return createBinding(source, nullptr, target, nullptr); }
	static PtrBinding createBinding(PtrNode source, const char * sourceArgumentName, PtrNode target) { return createBinding(source, sourceArgumentName, target, nullptr); }
	static PtrBinding createBinding(PtrNode source, PtrNode target, const char * targetArgumentName) { return createBinding(source, nullptr, target, targetArgumentName); }

	const char * getSourceArgumentName() const { return m_sourceArgumentName.c_str(); }
	const char * getTargetArgumentName() const { return m_targetArgumentName.c_str(); }

	auto getSourceNode() const { return m_source.lock(); }
	auto getTargetNode() const { return m_target.lock(); }
	bool is_valid() const { return (getSourceNode() && getTargetNode()); }

private:
	Binding(TArgumentType type, PtrNode source, const char * sourceArgumentName,
		PtrNode target, const char * targetArgumentName)
		: m_source(source), m_target(target), m_sourceArgumentName(sourceArgumentName), m_targetArgumentName(targetArgumentName)
	{

	};

	std::weak_ptr <INode> m_source, m_target;
	std::string m_sourceArgumentName,
		m_targetArgumentName;
};


#endif
