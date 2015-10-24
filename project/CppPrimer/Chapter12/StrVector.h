#ifndef STRVECTOR_H
#define STRVECTOR_H

#include <string>
#include <memory>

namespace chapter12
{

class StrVector
{
public:
    StrVector();
    StrVector(const StrVector &rf);
    StrVector& operator = (const StrVector &rf);
    ~StrVector();

    void pushBack(const std::string &str);
    std::size_t size() const;
    std::size_t capacity() const;
    std::string* begin() const;
    std::string* end() const;
    void resize(std::size_t n);
    void reserve(std::size_t n);

private:
    void checkAndAlloc();
    void reallocate();
    std::pair<std::string*, std::string*> allocAndCopy(const std::string* begin, const std::string* end);
    void free();

private:
    std::string *_elements;
    std::string *_firstFree;
    std::string *_cap;
    static std::allocator<std::string> _alloc;
};


}



#endif // STRVECTOR_H
