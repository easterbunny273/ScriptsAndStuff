#include <iostream>

#include "INode.h"
#include "TestNodes.h"

#include "GraphEvaluator.h"

PtrNode startNode(new ConstIntNode(5));
PtrNode startNode2(new ConstIntNode(42));
PtrNode startNode3(new ConstIntNode(9));
PtrNode addNode(new AddIntNode);
PtrNode addNode2(new AddIntNode);
PtrNode outputNode1(new IntOutputNode("node1"));
PtrNode outputNode2(new IntOutputNode("node2"));

// TODO:
// Note, nodes with multiple output bindings for 1 argument (e.g., [cur] result ->  [next1] in, [cur] result -> [next2] in) do not work 
// at the moment !

int main(int argc, char **argv)
{
	GraphEvaluator evaluator;

	auto outputSyncLock1 = evaluator.createSyncLock();
	auto outputSyncLock2 = evaluator.createSyncLock();

	evaluator.addNode(startNode);
	evaluator.addNode(startNode2);
	evaluator.addNode(startNode3);
	evaluator.addNode(addNode, outputSyncLock2);
	evaluator.addNode(addNode2, outputSyncLock2);
	evaluator.addNode(outputNode1, outputSyncLock1);
	evaluator.addNode(outputNode2, outputSyncLock1);
	evaluator.addBinding(Binding::createBinding(startNode, addNode, "lhs"));
	evaluator.addBinding(Binding::createBinding(startNode2, addNode, "rhs"));
	evaluator.addBinding(Binding::createBinding(startNode3, addNode2, "rhs"));
	evaluator.addBinding(Binding::createBinding(startNode2, addNode2, "lhs"));
	evaluator.addBinding(Binding::createBinding(addNode, outputNode1));
	evaluator.addBinding(Binding::createBinding(addNode2, outputNode2));

	evaluator.evaluate_threaded();
	//evaluator.evaluate();

	return 0;
}