#ifndef SALES_DATA_H
#define SALES_DATA_H

#include <iostream>
#include <string>

namespace chapter7
{

class SalesData
{
public:
    std::string isbn();
    void combine(const SalesData &rf);
    friend std::istream& read(std::istream &in, SalesData &total);
    friend std::ostream& print(std::ostream &out, SalesData &total);

private:
    std::string _bookNo;
    unsigned    _unitSold;
    double      _revenue;
};

void test();
std::istream& read(std::istream &in, SalesData &total);
std::ostream& print(std::ostream &out, SalesData &total);

}

#endif
