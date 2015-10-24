#ifndef SOURCE_MANAGER_H
#define SOURCE_MANAGER_H

#include <string>

namespace chapter12
{

class ValueManager
{
public:
    ValueManager(const std::string &s = std::string(""))
        : _ps(new std::string(s)), _i(0) {}
    ValueManager(const ValueManager &rf)
        : _ps(new std::string(*rf._ps)), _i(rf._i) {}
    ValueManager& operator = (const ValueManager &rf);
    ~ValueManager() { delete _ps;}

private:
    std::string *_ps;
    int _i;
};

class PointerManager
{
public:
    PointerManager(const std::string &s = std::string(""))
        : _ps(new std::string(s)), _i(0), _rfCount(new std::size_t(1)) {}
    PointerManager(const PointerManager &rf)
        : _ps(rf._ps), _i(rf._i), _rfCount(rf._rfCount) { ++*_rfCount; }
    PointerManager& operator = (const PointerManager &rf);
    ~PointerManager();

private:
    std::string *_ps;
    int _i;
    std::size_t *_rfCount;
};

}

#endif
