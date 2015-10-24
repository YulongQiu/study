#ifndef SCREEN_H
#define SCREEN_H

#include <string>

namespace chapter7
{

class Screen
{
public:
    typedef std::string::size_type pos;
    Screen();
    Screen(pos height, pos width, char c);
    ~Screen();
    char get() const { return _contents[_cursor]; }
    inline char get(pos row, pos col) const;
    Screen& move(pos row, pos col);
    void someMember() const;
    pos size() const;
    void print();

private:
    pos _cursor = 0;
    pos _height = 0;
    pos _width = 0;
    std::string _contents;
    mutable std::size_t _accessNum;
};

void screenTest();
}


#endif // SCREEN_H
