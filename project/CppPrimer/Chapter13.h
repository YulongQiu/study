#ifndef CHAPTER13_H
#define CHAPTER13_H

#include <string>

namespace chapter13
{

class Point
{
public:
    Point() = default;
    Point(const Point &pt);
};

void test13_4();


class HasPtr
{
    friend void swap(HasPtr &left, HasPtr &right);
public:
    HasPtr(const std::string &s = std::string(), int num = 0)
        : ps(new std::string(s)), i(num) {}
    HasPtr(const HasPtr &other);
    ~HasPtr();
    HasPtr& operator=(const HasPtr &other);
    void print();

private:
    std::string *ps;
    int i;
};

void swap(HasPtr &left, HasPtr &right);
void test13_30();


class Message
{
    friend class Folder;
public:
    explicit Message(const std::string &str = "")
        : contents(str) {}
    Message(const Message& msg);
    Message& operator= (const Message &msg);
    ~Message();

    void save();
    void remove();

private:
    void add_to_Folders(const Message& msg);
    void remove_from_Folders();

private:
    std::string contents;
    std::set<Folder*> folders;
};

class Folder
{

};

} // namespace

#endif // CHAPTER13_H
