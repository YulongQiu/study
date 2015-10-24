#include <iostream>
#include "SalesData.h"

using namespace std;

namespace chapter7
{

std::string SalesData::isbn()
{
    return _bookNo;
}

void SalesData::combine(const SalesData &rf)
{
    _unitSold += rf._unitSold;
    _revenue += rf._revenue;
}

std::istream& read(std::istream &in, SalesData &total)
{
    in >> total._bookNo >> total._unitSold >> total._revenue;
    return in;
}

std::ostream& print(std::ostream &out, SalesData &total)
{
    out << total._bookNo << " " << total._unitSold << " " << total._revenue;
    return out;
}


void test()
{
    SalesData total;

    if (read(cin, total))
    {
        SalesData trans;
        while (read(cin, trans))
        {
            if (total.isbn() == trans.isbn())
            {
                total.combine(trans);
            }
            else
            {
                print(cout, total) << endl;
                total = trans;
            }
        }
        print(cout, total) << endl;
    }
    else
    {
        cerr << "No data?!" << endl;
    }
}

}

