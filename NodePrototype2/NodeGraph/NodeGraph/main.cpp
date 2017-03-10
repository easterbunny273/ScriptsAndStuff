#include <iostream>
#include <vector>

#include "IVariable.h"
#include "BasicNodes.h"
#include "NodeNetwork.h"

using namespace std::literals;

int main(int argc, char ** argv)
{
	auto testVec = new Variable<std::vector<int>>();
	IVariable * test = new Variable<int>();

	if (test->hashCode() == Variable<int>::StaticHashCode())
	{
		Variable<int> * test2 = static_cast<Variable<int>*>(test);
		test2->value = 3;


		int x = test2->value;
		int y = x + 12;
	}

	auto testNode1 = std::make_shared<Output_Integer_Node>("testNode1");
	auto testNode2 = std::make_shared<PrintInteger_Node>("testNode2");
	auto testNode3 = std::make_shared<MultipleOutTestNode>("testNode3");

	NodeNetwork testNetwork;
	testNetwork.addNode(testNode1);
	testNetwork.addNode(testNode2);
	testNetwork.addNode(testNode3);
	testNetwork.addConnection(NodeNetwork::TConnection(testNode1, "output"s, testNode2, "input"s));
	testNetwork.removeConnection(testNode2, "input"s);
	testNetwork.addConnection(NodeNetwork::TConnection(testNode3, "output2"s, testNode2, "input"s));

	std::cout << "Hello World" << std::endl;
	return 0;
}