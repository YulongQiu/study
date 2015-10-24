#include <iostream>
#include "SalesItem.h"

namespace chapter1
{

SalesItem::SalesItem()
{

}

SalesItem::~SalesItem()
{

}

std::string SalesItem::isbn()
{
    return _ISBN;
}

SalesItem& SalesItem::operator +=(const SalesItem &item)
{
    _price = (_num * _price + item._num * item._price) / (_num + item._num);
    _num += item._num;
    return *this;
}

std::istream& operator >>(std::istream &isr, SalesItem &item)
{
    isr >> item._ISBN >> item._num >> item._price;
    return isr;
}

std::ostream& operator <<(std::ostream &osr, const SalesItem &item)
{
    osr << item._ISBN << " " << item._num << " "
        << item._num * item._price << " " << item._price;
    return osr;
}

int SalesItem_test()
{
    SalesItem total;
    if (std::cin >> total)
    {
        SalesItem trans;
        while (std::cin >> trans)
        {
            if (total.isbn() == trans.isbn())
            {
                total += trans;
            }
            else
            {
                std::cout << total << std::endl;
                total = trans;
            }
        }
        std::cout << total << std::endl;
    }
    else
    {
        std::cerr << "No data?!" << std::endl;
        return -1;
    }
    return 0;
}

}


