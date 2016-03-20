#include "exposedclasstest1.h"

#include <iostream>

ExposedClassTest1::ExposedClassTest1(QObject *parent) : QObject(parent)
{
    // nothing to do
}

bool ExposedClassTest1::printHello()
{
    std::cout << "print hello!" << std::endl;
    return true;
}
