#include "StrVector.h"

namespace chapter12
{
std::allocator<std::string> StrVector::_alloc;

StrVector::StrVector()
    : _elements(nullptr), _firstFree(nullptr), _cap(nullptr)
{

}

StrVector::StrVector(const StrVector &rf)
{
    auto data = allocAndCopy(rf.begin(), rf.end());
    _elements = data.first;
    _firstFree = _cap = data.second;
}

StrVector& StrVector::operator = (const StrVector &rf)
{
    auto data = allocAndCopy(rf.begin(), rf.end());
    free();
    _elements = data.first;
    _firstFree = _cap = data.second;

    return *this;
}

StrVector::~StrVector()
{
    free();
}

void StrVector::pushBack(const std::string &str)
{
    checkAndAlloc();
    _alloc.construct(_firstFree++, str);
}

std::size_t StrVector::size() const
{
    return _firstFree - _elements;
}

std::size_t StrVector::capacity() const
{
    return _cap - _elements;
}

std::string* StrVector::begin() const
{
    return _elements;
}

std::string* StrVector::end() const
{
    return _firstFree;
}

void StrVector::resize(std::size_t n)
{
    if (capacity() < n)
    {
        auto data = _alloc.allocate(n);
        auto dest = data;
        auto elem = _elements;
        for (std::size_t i = 0; i != size(); ++i)
        {
            _alloc.construct(dest++, std::move(*elem++));
        }

        for (std::size_t i = size(); i != n; ++i)
        {
            _alloc.construct(dest++, "");
        }
        free();
        _elements = data;
        _firstFree = dest;
        _cap = _elements + n;
    }
}

void StrVector::reserve(std::size_t n)
{
    if (capacity() < n)
    {
        auto data = _alloc.allocate(n);
        auto dest = data;
        auto elem = _elements;
        for (std::size_t i = 0; i != size(); ++i)
        {
            _alloc.construct(dest++, std::move(*elem++));
        }
        free();
        _elements = data;
        _firstFree = dest;
        _cap = _elements + n;
    }
}

void StrVector::checkAndAlloc()
{
    if (_firstFree == _cap)
    {
        reallocate();
    }
}

void StrVector::reallocate()
{
    auto newCapacity = size() ? 2 * size() : 1;
    auto newData = _alloc.allocate(newCapacity);
    auto dest = newData;
    auto elem = _elements;
    for (size_t i = 0; i != size(); ++i)
    {
        _alloc.construct(dest++, std::move(*elem++));
    }
    free();
    _elements = newData;
    _firstFree = dest;
    _cap = _elements + newCapacity;
}

std::pair<std::string*, std::string*> StrVector::allocAndCopy(const std::string* begin, const std::string* end)
{
    auto data = _alloc.allocate(end - begin);

    return {data, uninitialized_copy(begin, end, data)};
}

void StrVector::free()
{
    if (_elements)
    {
        for (auto p = _firstFree; p != _elements;)
        {
            _alloc.destroy(--p);
        }
        _alloc.deallocate(_elements, _cap - _elements);
    }
}

}


