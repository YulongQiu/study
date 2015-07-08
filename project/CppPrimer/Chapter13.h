#ifndef CHAPTER13_H
#define CHAPTER13_H

#include <string>

namespace chapter13
{

class Point
{
public:
    Point() = default;
    Point(const Point &pt);
};

void test13_4();


class HasPtr
{
public:
    HasPtr(const std::string &s = std::string())
        : ps(new std::string(s)), i(0) {}
    HasPtr(const HasPtr &other);
    ~HasPtr();
    HasPtr& operator=(const HasPtr &other);

private:
    std::string *ps;
    int i;
};


} // namespace

#endif // CHAPTER13_H
