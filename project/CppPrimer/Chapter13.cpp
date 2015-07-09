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

void HasPtr::print()
{
    std::cout << *ps << " " << i << std::endl;
}

void swap(HasPtr &left, HasPtr &right)
{
    using std::swap;
    swap(left.ps, right.ps);
    swap(left.i, right.i);
    std::cout << "swap" << std::endl;
}

void test13_30()
{
    HasPtr hasPtr1("hello", 0);
    HasPtr hasPtr2("world", 1);
    swap(hasPtr1, hasPtr2);
    hasPtr1.print();
    hasPtr2.print();
}

} // namespace
