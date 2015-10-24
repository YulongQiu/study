#ifndef INCLASSINITVALUE_H
#define INCLASSINITVALUE_H

#include <iostream>

namespace chapter7
{

class A
{
public:
    A(int a) { std::cout << a << std::endl;}

};

class B
{
public:
    B(int a) : _a(a){}

private:
    A _a = A(0);
};

void InClassInitValueTest();

}


#endif // INCLASSINITVALUE_H
