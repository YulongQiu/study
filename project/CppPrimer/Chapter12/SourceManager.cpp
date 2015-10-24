#include "SourceManager.h"

namespace chapter12
{

ValueManager& ValueManager::operator = (const ValueManager &rf)
{
    auto temp = new std::string(*rf._ps);
    delete _ps;
    _ps = temp;
    _i = rf._i;

    return *this;
}


PointerManager& PointerManager::operator = (const PointerManager &rf)
{
    ++*rf._rfCount;
    if (--*_rfCount == 0)
    {
        delete _ps;
        delete _rfCount;
    }

    _ps = rf._ps;
    _i = rf._i;
    _rfCount = rf._rfCount;

    return *this;
}

PointerManager::~PointerManager()
{
    if (--*_rfCount == 0)
    {
        delete _ps;
        delete _rfCount;
    }
}

}
