#include <iostream>
#include "Test.h"

namespace chapter2
{

void typeAliasTest()
{
    char cha = 'a';
    char chb = 'b';
    typedef char *pstring;

    const pstring cstr = &cha;
//    cstr = &chb;
//    *cstr = 'c';

    const char *ps = &cha;
//    ps = &chb;
//    *ps = 'c';

    const pstring *pps = &cstr;
//    pps = &ps;
//    **pps = 'c';


    // cstr: a const pointer which point to char
    // ps:   a pointer whitch point to const char
    // pps:  a pointer whitch point to a const pointer(point to char)

}

void autoTest()
{
    int i = 0, &r = i;
    auto a = r;

    const int ci = i, &cr = ci;
    auto b = ci;
    auto c = cr;
    auto d = &i;
    auto e = &ci;

    const auto f = ci;

    auto &g = ci;
//    auto &h = 42;
    const auto &z = 42;

    auto k = ci, &l = i;
    auto &m = ci, *p = &ci;
//    auto &n = i, *p2 = &ci;
}
}

