#include <iostream>
#include "Screen.h"

using namespace std;

namespace chapter7
{
Screen::Screen()
{

}

Screen::Screen(pos height, pos width, char c)
    : _height(height), _width(width), _contents(height * width, c)
{

}

Screen::~Screen()
{

}

char Screen::get(pos row, pos col) const
{
    if ((row < _width) && (col < _height))
    {
        pos r = row * _width;
        return _contents[r + col];
    }
    return ' ';
}

inline Screen& Screen::move(pos row, pos col)
{
    pos r = row * _width;
    _cursor = r + col;
    return *this;
}

void Screen::someMember() const
{
    _accessNum++;
}

Screen::pos Screen::size() const
{
    return _width * _height;
}

void Screen::print()
{
    cout << _cursor << endl;
}

void screenTest()
{
    Screen screen(2, 2, 'a');
    screen.print();

}

}

