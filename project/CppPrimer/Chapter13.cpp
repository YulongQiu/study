#include "Chapter13.h"

#include <iostream>

namespace chapter13
{

Point::Point(const Point &pt)
{
    std::cout << "call copy_assignment function." << std::endl;
}

Point global;

Point foo_bar(Point arg)    // 1
{
    Point local = arg;
    Point *heap = new Point(global);    // 2
    *heap = local;
    Point pa[4] = {local, *heap};     // 3, 4

    return *heap;   // 5
}

void test13_4()
{
    foo_bar(Point());
}


HasPtr::HasPtr(const HasPtr &other)
    : ps(new std::string(*(other.ps))), i(other.i)
{

}

HasPtr::~HasPtr()
{
    delete ps;    
}

HasPtr& HasPtr::operator=(const HasPtr &other)
{
    std::string *temp = new std::string(*(other.ps));
    delete ps;
    ps = temp;
    i = other.i;

    return *this;
}


} // namespace
