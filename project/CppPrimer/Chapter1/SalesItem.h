#ifndef SALESITEM_H
#define SALESITEM_H

#include <string>
#include <iostream>

namespace chapter1
{

class SalesItem
{
public:
    SalesItem();
    ~SalesItem();

    std::string isbn();
    SalesItem& operator +=(const SalesItem &item);
    friend std::istream& operator >>(std::istream &isr, SalesItem &item);
    friend std::ostream& operator <<(std::ostream &osr, const SalesItem &item);

private:
    std::string _ISBN;
    int _num;
    float _price;

};

std::istream& operator >>(std::istream &isr, SalesItem &item);
std::ostream& operator <<(std::ostream &osr, const SalesItem &item);

int SalesItem_test();
}



#endif // SALESITEM_H
